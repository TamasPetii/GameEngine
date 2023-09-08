#include "Mesh.h"

Mesh::Mesh()
{
	m_Vao = new OpenGL::Classic::VertexArrayObject;
	m_Vbo = new OpenGL::Classic::VertexBufferObject;
	m_Ibo = new OpenGL::Classic::IndexBufferObject;
}

Mesh::Mesh(const Mesh& other)
{
	std::cout << "Mesh Copy Constructor!" << std::endl;
	m_IndexCount = other.m_IndexCount;
	m_VertexCount = other.m_VertexCount;
	m_Vao = new OpenGL::Classic::VertexArrayObject;
	m_Vbo = new OpenGL::Classic::VertexBufferObject;
	m_Ibo = new OpenGL::Classic::IndexBufferObject;
}

Mesh::~Mesh()
{
	delete m_Vao;
	delete m_Vbo;
	delete m_Ibo;
}
void Mesh::Render() const
{
	m_Vao->Bind();
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
	m_Vao->UnBind();
}

void Mesh::UploadToGpu(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
	m_VertexCount = vertices.size();
	m_IndexCount = indices.size();

	m_Vao->Bind();
	m_Vbo->AttachData(vertices, OpenGL::STATIC);
	m_Ibo->AttachData(indices, OpenGL::STATIC);

	m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 0, 3, GL_FLOAT, sizeof(Vertex), nullptr);
	m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 1, 3, GL_FLOAT, sizeof(Vertex), (const void*)(1 * sizeof(glm::vec3)));
	m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 2, 3, GL_FLOAT, sizeof(Vertex), (const void*)(2 * sizeof(glm::vec3)));
	m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 3, 3, GL_FLOAT, sizeof(Vertex), (const void*)(3 * sizeof(glm::vec3)));
	m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 4, 2, GL_FLOAT, sizeof(Vertex), (const void*)(4 * sizeof(glm::vec3)));

	m_Vao->UnBind();
	m_Vbo->UnBind();
	m_Ibo->UnBind();
}