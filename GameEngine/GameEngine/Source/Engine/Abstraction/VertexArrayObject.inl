#include "VertexArrayObject.h"

namespace OpenGL::Classic
{
	template<typename T>
	void VertexArrayObject::LinkAttribute(const DataBufferObject<T>* buffer, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const
	{
		buffer->Bind();
		glEnableVertexAttribArray(layout);
		glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
		buffer->UnBind();
	}

	template<typename T>
	void VertexArrayObject::LinkAttributeI(const DataBufferObject<T>* buffer, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const
	{
		buffer->Bind();
		glEnableVertexAttribArray(layout);
		glVertexAttribIPointer(layout, size, type, stride, offset);
		buffer->UnBind();
	}
}