#include "Mesh.h"

Mesh::Mesh()
{
	m_Vao = new OpenGL::Classic::VertexArrayObject;
	m_Vbo = new OpenGL::Classic::VertexBufferObject;
	m_Ibo = new OpenGL::Classic::IndexBufferObject;
	m_Mbo = new OpenGL::Classic::MatrixBufferObject;
}

Mesh::Mesh(const Mesh& other)
{
	std::cout << "Mesh Copy Constructor!" << std::endl;
	m_IndexCount = other.m_IndexCount;
	m_VertexCount = other.m_VertexCount;
	m_Vao = new OpenGL::Classic::VertexArrayObject;
	m_Vbo = new OpenGL::Classic::VertexBufferObject;
	m_Ibo = new OpenGL::Classic::IndexBufferObject;
	m_Mbo = new OpenGL::Classic::MatrixBufferObject;
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

void Mesh::RenderInstanced(unsigned int instanceCount) const
{
	m_Vao->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr, instanceCount);
	m_Vao->UnBind();
}

void Mesh::UploadToGpu(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
	m_VertexCount = vertices.size();
	m_IndexCount = indices.size();

	m_Vao->Bind();
	m_Vbo->AttachData(vertices, OpenGL::STATIC);
	m_Ibo->AttachData(indices, OpenGL::STATIC);
	m_Mbo->AttachData(std::vector<glm::mat4>(1, glm::mat4(1)), OpenGL::STATIC);

	m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 0, 3, GL_FLOAT, sizeof(Vertex), nullptr);
	m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 1, 3, GL_FLOAT, sizeof(Vertex), (const void*)(1 * sizeof(glm::vec3)));
	m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 2, 3, GL_FLOAT, sizeof(Vertex), (const void*)(2 * sizeof(glm::vec3)));
	m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 3, 3, GL_FLOAT, sizeof(Vertex), (const void*)(3 * sizeof(glm::vec3)));
	m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 4, 2, GL_FLOAT, sizeof(Vertex), (const void*)(4 * sizeof(glm::vec3)));

	m_Vao->LinkAttribute(m_Mbo->Get_BufferId(), 5, 4, GL_FLOAT, sizeof(glm::mat4), (const void*)(0 * sizeof(glm::vec4)));
	m_Vao->LinkAttribute(m_Mbo->Get_BufferId(), 6, 4, GL_FLOAT, sizeof(glm::mat4), (const void*)(1 * sizeof(glm::vec4)));
	m_Vao->LinkAttribute(m_Mbo->Get_BufferId(), 7, 4, GL_FLOAT, sizeof(glm::mat4), (const void*)(2 * sizeof(glm::vec4)));
	m_Vao->LinkAttribute(m_Mbo->Get_BufferId(), 8, 4, GL_FLOAT, sizeof(glm::mat4), (const void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);

	m_Vao->UnBind();
	m_Vbo->UnBind();
	m_Ibo->UnBind();
}