#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
{
	Create(width, height);
}

FrameBuffer::~FrameBuffer()
{
	Delete();
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, mBufferId);
	glViewport(0, 0, mWidth, mHeight);
}

void FrameBuffer::UnBind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Create(unsigned int width, unsigned int height)
{
	mWidth = width;
	mHeight = height;

	//Create FrameBuffer
	glGenFramebuffers(1, &mBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, mBufferId);

	//Create and specify Texture
	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Attach Texture to FrameBuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTextureId, 0);

	//Create and specify DepthBuffer
	glGenRenderbuffers(1, &mDepthId);
	glBindRenderbuffer(GL_RENDERBUFFER, mDepthId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthId);

	//Check Errors
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("Error occurred while creating frame buffer!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Resize(unsigned int width, unsigned int height)
{
	Delete();
	Create(width, height);
}

void FrameBuffer::Delete()
{
	glDeleteFramebuffers(1, &mBufferId);
	glDeleteRenderbuffers(1, &mDepthId);
	glDeleteTextures(1, &mTextureId);
}
