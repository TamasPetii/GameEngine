#include "VertexArrayObject.h"

namespace OpenGL::Dsa
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
		glCreateVertexArrays(1, &m_VertexArrayId);
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
		glEnableVertexArrayAttrib(m_VertexArrayId, layout);
		glVertexArrayAttribBinding(m_VertexArrayId, layout, 0);
		glVertexArrayAttribFormat(m_VertexArrayId, layout, size, type, GL_FALSE, *(GLuint*)offset);
	}

	void VertexArrayObject::LinkAttributeI(GLuint bufferId, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const
	{
		glEnableVertexArrayAttrib(m_VertexArrayId, layout);
		glVertexArrayAttribBinding(m_VertexArrayId, layout, 0);
		glVertexArrayAttribIFormat(m_VertexArrayId, layout, size, type, *(GLuint*)offset);
	}
}