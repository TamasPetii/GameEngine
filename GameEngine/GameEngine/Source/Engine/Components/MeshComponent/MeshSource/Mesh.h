#pragma once
#include "../../../Abstractions/OpenGL.h"
#include "Renderable.h"


class Mesh : public Renderable
{
public:
	Mesh();
	Mesh(const Mesh& other);
	~Mesh();
	void Render() const override;
protected:
	void UploadToGpu(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
protected:
	GLuint m_IndexCount;
	GLuint m_VertexCount;
	OpenGL::Classic::VertexArrayObject* m_Vao;
	OpenGL::Classic::VertexBufferObject* m_Vbo;
	OpenGL::Classic::IndexBufferObject* m_Ibo;
};

