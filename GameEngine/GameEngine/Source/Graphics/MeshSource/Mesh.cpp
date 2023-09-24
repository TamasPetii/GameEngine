#include "Mesh.h"

Mesh::Mesh()
{
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

void Mesh::UploadToGpu(bool IsReuploaded)
{
	m_VertexCount = m_Vertices.size();
	m_IndexCount = m_Indices.size();

	m_Vao->Bind();
	m_Vbo->AttachData(m_Vertices, OpenGL::STATIC);
	m_Ibo->AttachData(m_Indices, OpenGL::STATIC);

	if (!IsReuploaded)
	{
		m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 0, 3, GL_FLOAT, sizeof(Vertex), nullptr);
		m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 1, 3, GL_FLOAT, sizeof(Vertex), (const void*)(1 * sizeof(glm::vec3)));
		m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 2, 3, GL_FLOAT, sizeof(Vertex), (const void*)(2 * sizeof(glm::vec3)));
		m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 3, 3, GL_FLOAT, sizeof(Vertex), (const void*)(3 * sizeof(glm::vec3)));
		m_Vao->LinkAttribute(m_Vbo->Get_BufferId(), 4, 2, GL_FLOAT, sizeof(Vertex), (const void*)(4 * sizeof(glm::vec3)));
	}

	m_Vao->UnBind();
	m_Vbo->UnBind();
	m_Ibo->UnBind();

	m_Vertices.clear();
	m_Indices.clear();
}

void Mesh::GenerateTbn(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, bool isAveraged)
{
	glm::vec3 edge1 = vertex2.position - vertex1.position;
	glm::vec3 edge2 = vertex3.position - vertex1.position;

	glm::vec2 deltaUV1 = vertex2.texture - vertex1.texture;
	glm::vec2 deltaUV2 = vertex3.texture - vertex1.texture;

	float det = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	glm::vec3 tangent;
	tangent.x = det * (edge1.x * deltaUV2.y - edge2.x * deltaUV1.y);
	tangent.y = det * (edge1.y * deltaUV2.y - edge2.y * deltaUV1.y);
	tangent.z = det * (edge1.z * deltaUV2.y - edge2.z * deltaUV1.y);

	glm::vec3 bitangent;
	bitangent.x = det * (-edge1.x * deltaUV2.x + edge2.x * deltaUV1.x);
	bitangent.y = det * (-edge1.y * deltaUV2.x + edge2.y * deltaUV1.x);
	bitangent.z = det * (-edge1.z * deltaUV2.x + edge2.z * deltaUV1.x);

	glm::vec3 normal = glm::cross(edge1, edge2);

	tangent = glm::normalize(tangent);
	bitangent = glm::normalize(bitangent);
	normal = glm::normalize(normal);

	if (isAveraged)
	{
		vertex1.normal += normal;
		vertex2.normal += normal;
		vertex3.normal += normal;

		vertex1.tangent += tangent;
		vertex2.tangent += tangent;
		vertex3.tangent += tangent;

		vertex1.bitangent += bitangent;
		vertex2.bitangent += bitangent;
		vertex3.bitangent += bitangent;
	}
	else
	{
		vertex1.normal = normal;
		vertex2.normal = normal;
		vertex3.normal = normal;

		vertex1.tangent = tangent;
		vertex2.tangent = tangent;
		vertex3.tangent = tangent;

		vertex1.bitangent = bitangent;
		vertex2.bitangent = bitangent;
		vertex3.bitangent = bitangent;
	}
}

void Mesh::GenerateTbn(Vertex& vertex, bool isAveraged = false)
{
	glm::vec3 up = glm::vec3(0, 1, 0);
	if (glm::dot(up, vertex.normal) > 0.999f)
	{
		up = glm::vec3(1, 0, 0); // Choose another up vector
	}

	glm::vec3 tangent = glm::normalize(glm::cross(up, vertex.normal));
	glm::vec3 bitangent = glm::normalize(glm::cross(vertex.normal, tangent));
	
	if (isAveraged)
	{
		vertex.tangent += tangent;
		vertex.bitangent += bitangent;
	}
	else
	{
		vertex.tangent = tangent;
		vertex.bitangent = bitangent;
	}
}