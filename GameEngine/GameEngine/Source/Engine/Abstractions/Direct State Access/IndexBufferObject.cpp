#include "IndexBufferObject.h"

namespace OpenGL::Dsa
{
	IndexBufferObject::IndexBufferObject()
	{
		GenerateBuffer();
	}

	IndexBufferObject::~IndexBufferObject()
	{
		DeleteBuffer();
	}

	void IndexBufferObject::GenerateBuffer()
	{
		glCreateBuffers(1, &m_IndexBufferId);
	}

	void IndexBufferObject::DeleteBuffer()
	{
		glDeleteBuffers(1, &m_IndexBufferId);
	}

	void IndexBufferObject::AttachData(const std::vector<GLuint>& data, BufferDrawMode mode)
	{
		glNamedBufferData(m_IndexBufferId, data.size() * sizeof(GLuint), data.data(), mode);
	}

	void IndexBufferObject::AttachSubData(const std::vector<GLuint>& data, int startIndex)
	{
		glNamedBufferSubData(m_IndexBufferId, startIndex * sizeof(GLuint), data.size() * sizeof(GLuint), data.data());
	}
}