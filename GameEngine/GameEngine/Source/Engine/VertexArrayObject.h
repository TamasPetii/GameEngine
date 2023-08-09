#pragma once
#include <GLEW/glew.h>
#include <vector>

#include "Buffer.h"

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void Bind() const;
	void UnBind() const;

	template<typename T>
	void LinkAttribute(const DataBuffer<T>& buffer, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const;

	template<typename T>
	void LinkAttributeI(const DataBuffer<T>& buffer, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const;

	template<typename T>
	void LinkAttributeInstance(const DataBuffer<T>& buffer, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const;

	template<typename T>
	void LinkAttributeInstanceI(const DataBuffer<T>& buffer, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const;

private:
	GLuint mVertexArrayId;
};

#include "VertexArrayObject.inl"

