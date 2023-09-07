#include "FrameBufferObject.h"

namespace OpenGL::Classic
{
	FrameBufferObject::FrameBufferObject()
	{
		m_Width = 1024;
		m_Height = 1024;
		GenerateBuffer();
	}

	FrameBufferObject::~FrameBufferObject()
	{
		DeleteBuffer();
		m_Textures.clear();
		m_RenderBuffers.clear();
	}

	void FrameBufferObject::GenerateBuffer()
	{
		glGenFramebuffers(1, &m_FrameBufferId);
	}

	void FrameBufferObject::DeleteBuffer()
	{
		glDeleteFramebuffers(1, &m_FrameBufferId);

		for (auto& [name, pair] : m_Textures)
		{
			glDeleteTextures(1, &pair.first);
		}

		for (auto& [name, pair] : m_RenderBuffers)
		{
			glDeleteRenderbuffers(1, &pair.first);
		}
	}

	void FrameBufferObject::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
		glViewport(0, 0, m_Width, m_Height);
	}

	void FrameBufferObject::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBufferObject::ResizeBuffer(GLint width, GLint height)
	{
		m_Width = width;
		m_Height = height;
		DeleteBuffer();
		ReGenerateBuffer();
	}

	void FrameBufferObject::ReGenerateBuffer()
	{
		GenerateBuffer();

		for (auto& [name, pair] : m_Textures)
		{
			GLuint textureId = GenerateTexture(pair.second);
			pair.first = textureId;
		}

		for (auto& [name, pair] : m_RenderBuffers)
		{
			GLuint renderBufferId = GenerateRenderBuffer(pair.second);
			pair.first = renderBufferId;
		}

		std::vector<GLenum> attachments;
		for (auto& [name, pair] : m_Textures)
		{
			if (pair.second.enableWrite)
				attachments.push_back(pair.second.attachment);
		}
 
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);

		std::sort(attachments.begin(), attachments.end());
		glDrawBuffers(attachments.size(), attachments.data());

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw std::runtime_error("Error occurred while creating frame buffer!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBufferObject::AttachTexture(const std::string& name, const FboTextureInfo& info)
	{
		GLuint textureId = GenerateTexture(info);
		m_Textures[name] = std::make_pair(textureId, info);
	}

	GLuint FrameBufferObject::GenerateTexture(const FboTextureInfo& info)
	{
		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, info.internalFormat, m_Width, m_Height, 0, info.format, info.type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		

		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
		glFramebufferTexture(GL_FRAMEBUFFER, info.attachment, textureId, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return textureId;
	}

	void FrameBufferObject::AttachRenderBuffer(const std::string& name, const FboRenderBufferInfo& info)
	{
		GLuint renderBufferId = GenerateRenderBuffer(info);
		m_RenderBuffers[name] = std::make_pair(renderBufferId, info);
	}

	GLuint FrameBufferObject::GenerateRenderBuffer(const FboRenderBufferInfo& info)
	{
		GLuint renderBufferId;
		glGenRenderbuffers(1, &renderBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, info.component, m_Width, m_Height);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, info.attachment, GL_RENDERBUFFER, renderBufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return renderBufferId;
	}

	void FrameBufferObject::ClearBuffer()
	{
		Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		for (auto& [name, pair] : m_Textures)
		{
			if (pair.second.clearCallBack)
				pair.second.clearCallBack(pair.first);
		}	

		UnBind();
	}

	std::any FrameBufferObject::ReadPixelData(const std::string& name, GLint x, GLint y)
	{
		if (m_Textures.at(name).second.readCallBack)
		{
			Bind();
			std::any pixelData = m_Textures.at(name).second.readCallBack(x, y);
			UnBind();

			return pixelData;
		}
		else
		{
			throw std::runtime_error("No readCallBack in fbo");
		}
	}
}
