#include "BufferGL.h"

BufferGL::BufferGL()
{
	glCreateBuffers(1, &m_BufferID);
}

BufferGL::~BufferGL()
{
	glDeleteBuffers(1, &m_BufferID);
}

void BufferGL::BufferStorage(GLsizeiptr size, const void* data, GLbitfield mode)
{
	glNamedBufferStorage(m_BufferID, size, data, mode);
}

void BufferGL::BufferSubStorage(GLintptr offset, GLsizeiptr size, const void* data)
{
	glNamedBufferSubData(m_BufferID, offset, size, data);
}

void BufferGL::UnMapBuffer() const
{
	glUnmapNamedBuffer(m_BufferID);
}

void* BufferGL::MapBuffer(GLenum mode) const
{
	return glMapNamedBuffer(m_BufferID, mode);
}
