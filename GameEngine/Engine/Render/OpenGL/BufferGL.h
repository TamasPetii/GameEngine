#pragma once
#include "EngineApi.h"
#include <GL/glew.h>

class ENGINE_API BufferGL
{
public:
	BufferGL();
	virtual ~BufferGL();
	virtual void BindBufferBase(GLuint layout) const {};

	void UnMapBuffer();
	void MapBuffer(GLenum mode);
	void MapBufferRange(GLbitfield  mode = GL_NONE, GLintptr offset = -1, GLsizeiptr length = -1);
	void BufferData(GLsizeiptr size, const void* data, GLenum mode = GL_STATIC_DRAW);
	void BufferStorage(GLsizeiptr size, const void* data, GLbitfield mode = GL_NONE, GLbitfield mapFlags = GL_NONE);
	void BufferSubStorage(GLintptr offset, GLsizeiptr size, const void* data);

	GLuint GetBufferID() const { return m_BufferID; }
	void* GetBufferHandler() const { return m_BufferHandler; }

	void SetMapFlags(GLenum flag) { m_MapFlags = flag; }
	void SetStorageFlags(GLenum flag) { m_StorageFlags = flag; } 
protected:
	GLbitfield m_StorageFlags;
	GLbitfield m_MapFlags;
	GLuint m_BufferID;
	GLsizeiptr m_Size;
	void* m_BufferHandler = nullptr;
};

using VertexBufferGL = BufferGL;
using IndexBufferGL = BufferGL;