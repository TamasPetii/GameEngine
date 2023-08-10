#include "FrameBuffer.h"

////////////////////////////////
///////ColorDepthStencil////////
////////////////////////////////

void FrameBufferObject<FBO_ColorDepthStencil>::CreateBuffers()
{
	//Create FrameBuffer
	glGenFramebuffers(1, &mFrameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);

	//Create and specify Texture
	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Attach Texture to FrameBuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTextureId, 0);

	//Create and specify DepthBuffer
	glGenRenderbuffers(1, &mDepthStencilBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilBufferId);

	//Check Errors
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("Error occurred while creating frame buffer!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject<FBO_ColorDepthStencil>::DeleteBuffers()
{
	glDeleteFramebuffers(1, &mFrameBufferId);
	glDeleteRenderbuffers(1, &mDepthStencilBufferId);
	glDeleteTextures(1, &mTextureId);
}

void FrameBufferObject<FBO_ColorDepthStencil>::ClearBuffers()
{
	Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	UnBind();
}

////////////////////////////////
//////////IntegerDepth//////////
////////////////////////////////

void FrameBufferObject<FBO_IntegerDepth>::CreateBuffers()
{
	//Create FrameBuffer
	glGenFramebuffers(1, &mFrameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);

	//Create and specify Texture
	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, mWidth, mHeight, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Attach Texture to FrameBuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTextureId, 0);

	//Create and specify DepthBuffer
	glGenRenderbuffers(1, &mDepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, mDepthBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBufferId);

	//Check Errors
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("Error occurred while creating frame buffer!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject<FBO_IntegerDepth>::DeleteBuffers()
{
	glDeleteFramebuffers(1, &mFrameBufferId);
	glDeleteRenderbuffers(1, &mDepthBufferId);
	glDeleteTextures(1, &mTextureId);
}

void FrameBufferObject<FBO_IntegerDepth>::ClearBuffers()
{
	Bind();
	int value = -1;
	glClearTexImage(mTextureId, 0, GL_RED_INTEGER, GL_INT, &value);
	glClear(GL_DEPTH_BUFFER_BIT);
	UnBind();
}

int FrameBufferObject<FBO_IntegerDepth>::ReadPixelData(int x, int y)
{
	Bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	int pixelData;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
	UnBind();

	return pixelData;
}