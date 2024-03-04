#pragma once
#include "EngineApi.h"
#include <GL/glew.h>

class ENGINE_API BufferGL
{
public:
	BufferGL();
	virtual ~BufferGL();
	virtual void BindBufferBase(unsigned int layout) const {};

	void  UnMapBuffer() const;
	void* MapBuffer(GLenum mode) const;
	void  BufferData(GLsizeiptr size, const void* data, GLenum mode = GL_STATIC_DRAW);
	void  BufferStorage(GLsizeiptr size, const void* data, GLenum mode = GL_NONE);
	void  BufferSubStorage(GLintptr offset, GLsizeiptr size, const void* data);
	const GLuint GetBufferID() const { return m_BufferID; }
protected:
	GLuint m_BufferID;
};

using VertexBufferGL = BufferGL;
using IndexBufferGL = BufferGL;