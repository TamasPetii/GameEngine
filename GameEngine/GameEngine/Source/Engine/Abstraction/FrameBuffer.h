#pragma once
#include "Buffer.h"

enum FrameBufferType
{
	ColorBuffer,
	IntegerBuffer
};

class FrameBuffer
{
public:
	FrameBuffer(unsigned int width, unsigned int height, FrameBufferType type);
	~FrameBuffer();
	static int GenerateUniqueId();

	void Create(unsigned int width, unsigned int height);
	void Resize(unsigned int width, unsigned int height);
	void Delete();

	void Bind() const;
	void UnBind() const;
	int ReadPixel(int x, int y);

	inline unsigned int GetBufferId() const { return mBufferId; }
	inline unsigned int GetTextureId() const { return mTextureId; }
	inline unsigned int GetDepthId() const { return mDepthId; }
	inline unsigned int GetWidth() const { return mWidth; }
	inline unsigned int GetHeight() const { return mHeight; }
private:
	int mUniqueId;
	unsigned int mBufferId;
	unsigned int mTextureId;
	unsigned int mDepthId;
	unsigned int mWidth;
	unsigned int mHeight;

	FrameBufferType mBufferType;
	int mInternalFormat;
	unsigned int mFormat;
};

