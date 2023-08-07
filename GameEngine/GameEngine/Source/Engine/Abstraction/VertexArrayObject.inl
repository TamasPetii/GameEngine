#include "VertexArrayObject.h"

template<typename T>
void VertexArrayObject::LinkAttribute(const DataBuffer<T>& buffer, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const
{
	buffer.Bind();

	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);

	buffer.UnBind();

}

template<typename T>
void VertexArrayObject::LinkAttributeI(const DataBuffer<T>& buffer, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const
{
	buffer.Bind();

	glEnableVertexAttribArray(layout);
	glVertexAttribIPointer(layout, size, type, stride, offset);

	buffer.UnBind();
}

template<typename T>
void VertexArrayObject::LinkAttributeInstance(const DataBuffer<T>& buffer, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const
{
	buffer.Bind();

	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
	glVertexAttribDivisor(layout, 1);

	buffer.UnBind();
}

template<typename T>
void VertexArrayObject::LinkAttributeInstanceI(const DataBuffer<T>& buffer, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const
{
	buffer.Bind();

	glEnableVertexAttribArray(layout);
	glVertexAttribIPointer(layout, size, type, stride, offset);
	glVertexAttribDivisor(layout, 1);

	buffer.UnBind();
}