#include "FrameBuffer.h"

////////////////////////////////
////////////General/////////////
////////////////////////////////

template<typename T>
FrameBufferObjectBase<T>::FrameBufferObjectBase() :
	mFrameBufferId(0),
	mTextureId(0),
	mHeight(1024),
	mWidth(1024)
{
}

template<typename T>
FrameBufferObjectBase<T>::~FrameBufferObjectBase()
{
}

template<typename T>
void FrameBufferObjectBase<T>::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->mFrameBufferId);
	glViewport(0, 0, this->mWidth, this->mHeight);
}

template<typename T>
void FrameBufferObjectBase<T>::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

template<typename T>
unsigned int FrameBufferObjectBase<T>::GetTextureId()
{
	return this->mTextureId;
}

template<typename T>
void FrameBufferObjectBase<T>::ResizeBuffers(unsigned int width, unsigned int height)
{
	this->mWidth = width;
	this->mHeight = height;
	this->DeleteBuffers();
	this->CreateBuffers();
}