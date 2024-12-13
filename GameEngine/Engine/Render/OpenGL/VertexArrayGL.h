#pragma once
#include "EngineApi.h"
#include <GL/glew.h>
#include <memory>

class BufferGL;

class ENGINE_API VertexArrayGL
{
public:
	VertexArrayGL();
	virtual ~VertexArrayGL();
	void Bind();
	void UnBind();
	void AttachIndexBuffer(const std::unique_ptr<BufferGL>& indexBuffer);
	void AttachVertexBuffer(const std::unique_ptr<BufferGL>& vertexBuffer, GLsizei stride, GLuint bindingIndex = 0);
	void LinkAttribute(GLuint bindingIndex, GLuint attribIndex, GLint count, GLenum type, GLuint offset);
	void LinkAttributeI(GLuint bindingIndex, GLuint attribIndex, GLint count, GLenum type, GLuint offset);
	const GLuint GetVertexArrayID() const { return m_VertexArrayID; }
private:
	GLuint m_VertexArrayID;
};

