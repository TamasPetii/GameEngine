#pragma once
#include <GLEW/glew.h>
#include "Vertex.h"
#include <vector>

class FBO_ColorDepth {};
class FBO_ColorDepthStencil {};
class FBO_IntegerDepth {};

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
class FrameBufferObject<FBO_ColorDepthStencil> : public FrameBufferObjectBase<FBO_ColorDepthStencil>
{
public:
	FrameBufferObject() : mDepthStencilBufferId(0), FrameBufferObjectBase() { this->CreateBuffers(); }
	~FrameBufferObject() { this->DeleteBuffers(); }
	void CreateBuffers() override;
	void DeleteBuffers() override;
	void ClearBuffers() override;
private:
	unsigned int mDepthStencilBufferId;
};

template<>
class FrameBufferObject<FBO_IntegerDepth> : public FrameBufferObjectBase<FBO_IntegerDepth>
{
public:
	FrameBufferObject() : mDepthBufferId(0), FrameBufferObjectBase() { this->CreateBuffers(); }
	~FrameBufferObject() { this->DeleteBuffers(); }
	void CreateBuffers() override;
	void DeleteBuffers() override;
	void ClearBuffers() override;
	int ReadPixelData(int x, int y);
private:
	unsigned int mDepthBufferId;
};

#include "FrameBuffer.inl"
