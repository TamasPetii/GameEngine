#include "Buffer.h"

Buffer::Buffer()
{
	glGenBuffers(1, &mBufferId);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &mBufferId);
}

void IndexBufferObject::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);
}

void IndexBufferObject::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBufferObject::AttachData(const std::vector<unsigned int>& data, int mode)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), data.data(), mode);
}

void IndexBufferObject::AttachSubData(const std::vector<unsigned int>& data)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data.size() * sizeof(unsigned int), data.data());
}