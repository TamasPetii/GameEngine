#include "DataBufferObject.h"

namespace OpenGL::Dsa
{
	template<typename T>
	DataBufferObject<T>::DataBufferObject()
	{
		GenerateBuffer();
	}

	template<typename T>
	DataBufferObject<T>::~DataBufferObject()
	{
		DeleteBuffer();
	}

	template<typename T>
	void DataBufferObject<T>::GenerateBuffer()
	{
		glCreateBuffers(1, &this->m_DataBufferId);
	}

	template<typename T>
	void DataBufferObject<T>::DeleteBuffer()
	{
		glDeleteBuffers(1, &this->m_DataBufferId);
	}

	template<typename T>
	void DataBufferObject<T>::AttachData(const std::vector<T>& data, BufferDrawMode mode)
	{
		glNamedBufferData(m_DataBufferId, data.size() * sizeof(T), data.data(), mode);
	}

	template<typename T>
	void DataBufferObject<T>::AttachSubData(const std::vector<T>& data, int startIndex)
	{
		glNamedBufferSubData(m_DataBufferId, startIndex * sizeof(T), data.size() * sizeof(T));
	}

}