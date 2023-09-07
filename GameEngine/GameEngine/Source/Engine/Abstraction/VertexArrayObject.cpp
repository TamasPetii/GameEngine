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
}