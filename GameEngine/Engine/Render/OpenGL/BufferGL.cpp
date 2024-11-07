#include "BufferGL.h"

BufferGL::BufferGL()
{
	glCreateBuffers(1, &m_BufferID);
}

BufferGL::~BufferGL()
{
	glDeleteBuffers(1, &m_BufferID);
}

void BufferGL::BufferStorage(GLsizeiptr size, const void* data, GLbitfield mode, GLbitfield mapFlags)
{
	if (mapFlags != GL_NONE)
		m_MapFlags = mapFlags;

	m_Size = size;
	m_StorageFlags = mode;
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

void* BufferGL::MapBufferRange(GLbitfield mode, GLintptr offset, GLsizeiptr length)
{
	return glMapNamedBufferRange(m_BufferID, offset == -1 ? 0 : offset, length == -1 ? m_Size : length, mode == GL_NONE ? m_MapFlags : mode);
}
