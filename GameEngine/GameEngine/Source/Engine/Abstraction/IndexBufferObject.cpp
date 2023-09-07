#include "IndexBufferObject.h"

namespace OpenGL::Classic
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
		glGenBuffers(1, &m_IndexBufferId);
	}

	void IndexBufferObject::DeleteBuffer()
	{
		glDeleteBuffers(1, &m_IndexBufferId);
	}

	void IndexBufferObject::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
	}

	void IndexBufferObject::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBufferObject::AttachData(const std::vector<GLuint>& data, int mode)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), data.data(), mode);
	}

	void IndexBufferObject::AttachSubData(const std::vector<GLuint>& data, int startIndex)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, startIndex * sizeof(GLuint), data.size() * sizeof(GLuint), data.data());
	}
}