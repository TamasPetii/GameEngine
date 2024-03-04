#pragma once
#include <GL/glew.h>
#include <Render/OpenGL/BufferGL.h>

#include <vector>
#include <memory>

class VertexArrayGL
{
public:
	VertexArrayGL();
	~VertexArrayGL();
	void Bind();
	void UnBind();
	void AttachIndexBuffer(const std::unique_ptr<IndexBufferGL>& indexBuffer);
	void AttachVertexBuffer(const std::unique_ptr<VertexBufferGL>& vertexBuffer, GLsizei stride, GLuint bindingIndex = 0);
	void LinkAttribute(GLuint bindingIndex, GLuint attribIndex, GLint count, GLenum type, GLuint offset);
	void LinkAttributeI(GLuint bindingIndex, GLuint attribIndex, GLint count, GLenum type, GLuint offset);
	const GLuint GetVertexArrayID() const { return m_VertexArrayID; }
private:
	GLuint m_VertexArrayID;
};

