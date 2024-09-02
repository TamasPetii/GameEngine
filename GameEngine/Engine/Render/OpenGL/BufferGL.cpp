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
	m_Flags = mode;
	m_Size = size;
	glNamedBufferStorage(m_BufferID, size, data, mode);
}

void BufferGL::BufferData(GLsizeiptr size, const void* data, GLenum mode)
{
	glNamedBufferData(m_BufferID, size, data, mode);
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

void* BufferGL::MapBufferRange(int offset, int length)
{
	return glMapNamedBufferRange(m_BufferID, offset == -1 ? 0 : offset, length == -1 ? m_Size : length, m_Flags);
}
