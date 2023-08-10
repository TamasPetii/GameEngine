#pragma once
#include <GLEW/glew.h>
#include "Vertex.h"
#include <vector>

class FBO_ColorTexture {};
class FBO_IntegerTexture {};
class FBO_DepthTexture {};

class IFrameBufferObject 
{
public:
	virtual void Bind() = 0;
	virtual void UnBind() = 0;
	virtual void ResizeBuffers(unsigned int width, unsigned int height) = 0;
	virtual void CreateBuffers() = 0;
	virtual void DeleteBuffers() = 0;
	virtual void ClearBuffers() = 0;
	virtual unsigned int GetTextureId() = 0;
};

template <typename T>
class FrameBufferObjectBase : public IFrameBufferObject
{
public:
	FrameBufferObjectBase();
	~FrameBufferObjectBase();
	void Bind() override;
	void UnBind() override;
	void ResizeBuffers(unsigned int width, unsigned int height) override;
	unsigned int GetTextureId() override;
protected:
	unsigned int mFrameBufferId;
	unsigned int mTextureId;
	unsigned int mWidth;
	unsigned int mHeight;
};

template <typename T>
class FrameBufferObject : public FrameBufferObjectBase<T> {};

template<>
class FrameBufferObject<FBO_ColorTexture> : public FrameBufferObjectBase<FBO_ColorTexture>
{
public:
	FrameBufferObject() : mDepthStencilBufferId(0), FrameBufferObjectBase()
	{ 
		this->CreateBuffers();
	}
	~FrameBufferObject() { this->DeleteBuffers(); }
	void CreateBuffers() override;
	void DeleteBuffers() override;
	void ClearBuffers() override;
private:
	unsigned int mDepthStencilBufferId;
};

template<>
class FrameBufferObject<FBO_IntegerTexture> : public FrameBufferObjectBase<FBO_IntegerTexture>
{
public:
	FrameBufferObject() : mDepthBufferId(0), FrameBufferObjectBase() 
	{ 
		this->CreateBuffers();
	}
	~FrameBufferObject() { this->DeleteBuffers(); }
	void CreateBuffers() override;
	void DeleteBuffers() override;
	void ClearBuffers() override;
	int ReadPixelData(int x, int y);
private:
	unsigned int mDepthBufferId;
};

template<>
class FrameBufferObject<FBO_DepthTexture> : public FrameBufferObjectBase<FBO_DepthTexture>
{
public:
	FrameBufferObject() : FrameBufferObjectBase() 
	{ 
		this->CreateBuffers();
	}
	~FrameBufferObject() { this->DeleteBuffers(); }
	void CreateBuffers() override;
	void DeleteBuffers() override;
	void ClearBuffers() override;
};

#include "FrameBuffer.inl"
