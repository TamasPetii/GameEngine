#include "VertexArrayGL.h"
#include "Render/OpenGL/BufferGL.h"

VertexArrayGL::VertexArrayGL()
{
	glCreateVertexArrays(1, &m_VertexArrayID);
}

VertexArrayGL::~VertexArrayGL()
{
	glDeleteVertexArrays(1, &m_VertexArrayID);
}

void VertexArrayGL::AttachIndexBuffer(const std::unique_ptr<BufferGL>& indexBuffer)
{
	glVertexArrayElementBuffer(m_VertexArrayID, indexBuffer->GetBufferID());
}

void VertexArrayGL::AttachVertexBuffer(const std::unique_ptr<BufferGL>& vertexBuffer, GLsizei stride, GLuint bindingIndex)
{
	glVertexArrayVertexBuffer(m_VertexArrayID, bindingIndex, vertexBuffer->GetBufferID(), 0, stride);
}

void VertexArrayGL::LinkAttribute(GLuint bindingIndex, GLuint attribIndex, GLint count, GLenum type, GLuint offset)
{
	glEnableVertexArrayAttrib(m_VertexArrayID, attribIndex);
	glVertexArrayAttribFormat(m_VertexArrayID, attribIndex, count, type, GL_FALSE, offset);
	glVertexArrayAttribBinding(m_VertexArrayID, attribIndex, bindingIndex);
}

void VertexArrayGL::LinkAttributeI(GLuint bindingIndex, GLuint attribIndex, GLint count, GLenum type, GLuint offset)
{
	glEnableVertexArrayAttrib(m_VertexArrayID, attribIndex);
	glVertexArrayAttribIFormat(m_VertexArrayID, attribIndex, count, type, offset);
	glVertexArrayAttribBinding(m_VertexArrayID, attribIndex, bindingIndex);
}

void VertexArrayGL::Bind()
{
	glBindVertexArray(m_VertexArrayID);
}

void VertexArrayGL::UnBind()
{
	glBindVertexArray(0);
}