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

	void FrameBufferObject::ReGenerate()
	{
		DeleteBuffer();
		GenerateBuffer();
		ReGenerateTexture();
		ReGenerateRenderBuffer();
		
		//Todo Delete this part
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
}