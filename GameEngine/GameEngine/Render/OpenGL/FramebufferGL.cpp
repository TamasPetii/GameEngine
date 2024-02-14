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

void FramebufferGL::AttachTexture(const std::string& name, const TextureFboSpecGL& spec)
{
	GLsizei width  = spec.width != 0  ? spec.width  : m_Size.x;
	GLsizei height = spec.height != 0 ? spec.height : m_Size.y;

	GLuint textureID;
	glCreateTextures(spec.textureType, 1, &textureID);
	spec.paramTextureFunction(textureID);
	glTextureStorage2D(textureID, spec.layer, spec.internalFormat, width, height);
	glNamedFramebufferTexture(m_FramebufferID, spec.attachment, textureID, 0);

	if (m_Textures.find(name) == m_Textures.end())
		m_Textures[name] = std::make_pair(textureID, spec);
	else
		m_Textures[name].first = textureID;
}

void FramebufferGL::AttachRenderbuffer(const std::string& name, const RenderbufferFboSpecGL& spec)
{
	GLsizei width = spec.width != 0 ? spec.width : m_Size.x;
	GLsizei height = spec.height != 0 ? spec.height : m_Size.y;

	GLuint renderBufferID;
	glCreateRenderbuffers(1, &renderBufferID);
	glNamedRenderbufferStorage(renderBufferID, spec.component, spec.width, spec.height);
	glNamedFramebufferRenderbuffer(m_FramebufferID, spec.attachment, GL_RENDERBUFFER, renderBufferID);

	if (m_RenderBuffers.find(name) != m_RenderBuffers.end())
		m_RenderBuffers[name] = std::make_pair(renderBufferID, spec);
	else
		m_RenderBuffers[name].first = renderBufferID;
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
		glNamedFramebufferDrawBuffer(m_FramebufferID, m_Textures[name].second.attachment);
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
			attachments.push_back(m_Textures[name].second.attachment);
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
		AttachTexture(name, data.second);

	for (const auto& [name, data] : m_RenderBuffers)
		AttachRenderbuffer(name, data.second);
}

void FramebufferGL::DeleteResources()
{
	glDeleteFramebuffers(1, &m_FramebufferID);

	for (const auto& [name, data] : m_Textures)
		glDeleteTextures(1, &data.first);

	for (const auto& [name, data] : m_RenderBuffers)
		glDeleteRenderbuffers(1, &data.first);
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
		if (data.second.attachment >= GL_COLOR_ATTACHMENT0 &&
			data.second.attachment <= GL_COLOR_ATTACHMENT15 &&
			data.second.clearTextureFunction == nullptr)
			attachments.push_back(data.second.attachment);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
	glNamedFramebufferDrawBuffers(m_FramebufferID, attachments.size(), attachments.data());
	glViewport(0, 0, m_Size.x, m_Size.y);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (const auto& [name, data] : m_Textures)
	{
		if (data.second.clearTextureFunction)
		{
			glNamedFramebufferDrawBuffer(m_FramebufferID, data.second.attachment);
			data.second.clearTextureFunction(data.first, data.second);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const GLuint FramebufferGL::GetTextureID(const std::string& name)
{
	if (m_Textures.find(name) != m_Textures.end())
		return m_Textures[name].first;
	else
		return 0;
}

std::any FramebufferGL::ReadPixel(const std::string& name, GLint x, GLint y)
{
	if (m_Textures.find(name) != m_Textures.end() && m_Textures[name].second.readTextureFunction)
		return m_Textures[name].second.readTextureFunction(m_Textures[name].first, m_Textures[name].second, x, y);

	return std::any(nullptr);
}

void FramebufferGL::BindTexture(const std::string& name)
{
	glNamedFramebufferTexture(m_FramebufferID, m_Textures[name].second.attachment, m_Textures[name].first, 0);
}