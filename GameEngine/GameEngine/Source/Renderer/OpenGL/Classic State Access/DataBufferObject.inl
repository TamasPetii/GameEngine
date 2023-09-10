#include "DataBufferObject.h"

namespace OpenGL::Classic
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
		glGenBuffers(1, &this->m_DataBufferId);
	}

	template<typename T>
	void DataBufferObject<T>::DeleteBuffer()
	{
		glDeleteBuffers(1, &this->m_DataBufferId);
	}

	template<typename T>
	void DataBufferObject<T>::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->m_DataBufferId);
	}

	template<typename T>
	void DataBufferObject<T>::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template<typename T>
	void DataBufferObject<T>::AttachData(const std::vector<T>& data, BufferDrawMode mode)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->m_DataBufferId);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), mode);
	}

	template<typename T>
	void DataBufferObject<T>::AttachSubData(const std::vector<T>& data, int startIndex)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->m_DataBufferId);
		glBufferSubData(GL_ARRAY_BUFFER, startIndex * sizeof(T), data.size() * sizeof(T), data.data());
	}

}