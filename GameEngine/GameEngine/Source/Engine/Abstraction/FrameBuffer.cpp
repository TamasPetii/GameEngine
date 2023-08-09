#include "FrameBuffer.h"

int FrameBuffer::GenerateUniqueId()
{
	static int id = 0;
	return id++;
}

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, FrameBufferType type)
{
	mUniqueId = FrameBuffer::GenerateUniqueId();
	mBufferType = type;

	switch (mBufferType)
	{
	case FrameBufferType::ColorBuffer:
		mInternalFormat = GL_RGBA8;
		mFormat = GL_RGBA;
		break;
	case FrameBufferType::IntegerBuffer:
		mInternalFormat = GL_R32I;
		mFormat = GL_RED_INTEGER;
		break;
	}

	Create(width, height);
}

FrameBuffer::~FrameBuffer()
{
	Delete();
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);
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
	glGenFramebuffers(1, &mFrameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);

	//Create and specify Texture
	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, width, height, 0, mFormat, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Attach Texture to FrameBuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTextureId, 0);

	//Create and specify DepthBuffer
	glGenRenderbuffers(1, &mDepthStencilBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilBufferId);
	
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
	glDeleteFramebuffers(1, &mFrameBufferId);
	glDeleteRenderbuffers(1, &mDepthStencilBufferId);
	glDeleteTextures(1, &mTextureId);
}

int FrameBuffer::ReadPixel(int x, int y)
{
	FrameBuffer::Bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	int pixelData;
	glReadPixels(x, y, 1, 1, mFormat, GL_INT, &pixelData);
	FrameBuffer::UnBind();

	return pixelData;
}

void FrameBuffer::Clear()
{
	FrameBuffer::Bind();
	if (mBufferType == FrameBufferType::ColorBuffer)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	if (mBufferType == FrameBufferType::IntegerBuffer)
	{
		int value = -1;
		glClearTexImage(mTextureId, 0, GL_RED_INTEGER, GL_INT, &value);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	FrameBuffer::UnBind();
}