#include "Buffer.h"

template<typename T>
void DataBuffer<T>::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);
}

template<typename T>
void DataBuffer<T>::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
void DataBuffer<T>::AttachData(const std::vector<T>& data, int mode)
{
	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), mode);
}

template<typename T>
void DataBuffer<T>::AttachSubData(const std::vector<T>& data)
{
	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(T), data.data());
}