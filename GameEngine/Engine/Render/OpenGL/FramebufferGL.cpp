#include "FramebufferGL.h"

FramebufferGL::FramebufferGL()
{
	m_Size.x = 1024;
	m_Size.y = 768;
	glCreateFramebuffers(1, &m_FramebufferID);
}

FramebufferGL::FramebufferGL(GLint width, GLint height)
{
	m_Size.x = width;
	m_Size.y = height;
	glCreateFramebuffers(1, &m_FramebufferID);
}

FramebufferGL::~FramebufferGL()
{
	DeleteResources();
}

void FramebufferGL::Bind(GLenum mode)
{
	glBindFramebuffer(mode, m_FramebufferID);
	glViewport(0, 0, m_Size.x, m_Size.y);
}

void FramebufferGL::UnBind(GLenum mode)
{
	glBindFramebuffer(mode, 0);
}

void FramebufferGL::AttachTexture(const std::string& name, TextureSpecGL& spec)
{
	if (useFboSizeAsTexSize)
	{
		spec.width = m_Size.x;
		spec.height = m_Size.y;
	}

	std::shared_ptr<TextureGL> texture = std::make_shared<TextureGL>(spec);
	glNamedFramebufferTexture(m_FramebufferID, spec.attachment, texture->GetTextureID(), 0);
	texture->GenerateHandler();
	m_Textures[name] = texture;
}

void FramebufferGL::CheckCompleteness()
{
	if (glCheckNamedFramebufferStatus(m_FramebufferID, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Error occurred while creating frame buffer!");
}

void FramebufferGL::DeactivateTexture()
{
	glNamedFramebufferDrawBuffer(m_FramebufferID, GL_NONE);
}

void FramebufferGL::ActivateTexture(const std::string& name)
{
	if (m_Textures.find(name) != m_Textures.end())
		glNamedFramebufferDrawBuffer(m_FramebufferID, m_Textures[name]->GetTextureSpec().attachment);
}

void FramebufferGL::ActivateTexture(GLenum attachment)
{
	glNamedFramebufferDrawBuffer(m_FramebufferID, attachment);
}

void FramebufferGL::ActivateTextures(const std::vector<std::string>& names)
{
	std::vector<GLenum> attachments;

	for (auto& name : names)
	{
		if (m_Textures.find(name) != m_Textures.end())
			attachments.push_back(m_Textures[name]->GetTextureSpec().attachment);
	}

	glNamedFramebufferDrawBuffers(m_FramebufferID, attachments.size(), attachments.data());
}

void FramebufferGL::ActivateTextures(const std::vector<GLenum>& attachments)
{
	glNamedFramebufferDrawBuffers(m_FramebufferID, attachments.size(), attachments.data());
}

void FramebufferGL::RecreateResources()
{
	glCreateFramebuffers(1, &m_FramebufferID);

	for (const auto& [name, data] : m_Textures)
	{
		TextureSpecGL spec = data->GetTextureSpec();
		AttachTexture(name, spec);
	}
}

void FramebufferGL::DeleteResources()
{
	glDeleteFramebuffers(1, &m_FramebufferID);
}

void FramebufferGL::Resize(GLint width, GLint height)
{
	m_Size = { width , height };
	DeleteResources();
	RecreateResources();
	CheckCompleteness();
}

void FramebufferGL::Clear()
{
	std::vector<GLenum> attachments;
	for (const auto& [name, data] : m_Textures)
	{
		if (data->GetTextureSpec().attachment >= GL_COLOR_ATTACHMENT0 &&
			data->GetTextureSpec().attachment <= GL_COLOR_ATTACHMENT15 &&
			data->GetTextureSpec().clearTextureFunction == nullptr)
			attachments.push_back(data->GetTextureSpec().attachment);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
	glNamedFramebufferDrawBuffers(m_FramebufferID, attachments.size(), attachments.data());
	glViewport(0, 0, m_Size.x, m_Size.y);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (const auto& [name, data] : m_Textures)
	{
		if (data->GetTextureSpec().clearTextureFunction)
		{
			glNamedFramebufferDrawBuffer(m_FramebufferID, data->GetTextureSpec().attachment);
			data->GetTextureSpec().clearTextureFunction(data->GetTextureID(), data->GetTextureSpec());
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const GLuint FramebufferGL::GetTextureID(const std::string& name)
{
	if (m_Textures.find(name) != m_Textures.end())
		return m_Textures[name]->GetTextureID();
	else
		return 0;
}

std::any FramebufferGL::ReadPixel(const std::string& name, GLint x, GLint y)
{
	if (m_Textures.find(name) != m_Textures.end())
	{
		auto  texture = m_Textures[name];
		auto& spec = texture->GetTextureSpec();

		if (spec.readTextureFunction)
		{
			return spec.readTextureFunction(texture->GetTextureID(), spec, x, y);
		}
	}

	return std::any(nullptr);
}

void FramebufferGL::BindTexture(const std::string& name)
{
	glNamedFramebufferTexture(m_FramebufferID, m_Textures[name]->GetTextureSpec().attachment, m_Textures[name]->GetTextureID(), 0);
}