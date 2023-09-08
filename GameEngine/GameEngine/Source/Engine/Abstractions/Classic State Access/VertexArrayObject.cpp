#include "VertexArrayObject.h"

namespace OpenGL::Classic
{
	VertexArrayObject::VertexArrayObject()
	{
		GenerateVertexArray();
	}

	VertexArrayObject::~VertexArrayObject()
	{
		DeleteVertexArray();
	}

	void VertexArrayObject::GenerateVertexArray()
	{
		glGenVertexArrays(1, &m_VertexArrayId);
	}

	void VertexArrayObject::DeleteVertexArray()
	{
		glDeleteVertexArrays(1, &m_VertexArrayId);
	}

	void VertexArrayObject::Bind() const
	{
		glBindVertexArray(m_VertexArrayId);
	}

	void VertexArrayObject::UnBind() const
	{
		glBindVertexArray(0);
	}

	void VertexArrayObject::LinkAttribute(GLuint bufferId, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		glEnableVertexAttribArray(layout);
		glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexArrayObject::LinkAttributeI(GLuint bufferId, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		glEnableVertexAttribArray(layout);
		glVertexAttribIPointer(layout, size, type, stride, offset);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}