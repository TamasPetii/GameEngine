#pragma once
#include <GLEW/glew.h>
#include <vector>

#include "Vertex.h"

class Buffer
{
public:
	Buffer();
	virtual ~Buffer();
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	inline unsigned int GetBufferId() const { return mBufferId; }
protected:
	unsigned int mBufferId;
};

template<typename T>
class DataBuffer : public Buffer
{
public:
	DataBuffer() : Buffer() {}
	void Bind() const override;
	void UnBind() const override;
	void AttachData(const std::vector<T>& data, int mode);
	void AttachSubData(const std::vector<T>& data);
};

class IndexBufferObject : public Buffer
{
public:
	void Bind() const override;
	void UnBind() const override;
	void AttachData(const std::vector<unsigned int>& data, int mode);
	void AttachSubData(const std::vector<unsigned int>& data);
private:
};

using VertexBufferObject = DataBuffer<Vertex>;

#include "Buffer.inl"

