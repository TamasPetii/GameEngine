#include "Geometry.h"

Geometry::Geometry()
{
	m_Vao = std::make_unique<VertexArrayGL>();
	m_Ibo = std::make_unique<IndexBufferGL>();
	m_Vbo = std::make_unique<VertexBufferGL>();
	m_InstanceSsbo = std::make_unique<ShaderStorageBufferGL>();
	m_ShadowInstanceSsbo = std::make_unique<ShaderStorageBufferGL>();
}

void Geometry::Bind()
{
	m_Vao->Bind();
}

void Geometry::UnBind()
{
	m_Vao->UnBind();
}

void Geometry::GenerateBuffers()
{
	m_Vbo->BufferStorage(m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_NONE);
	m_Ibo->BufferStorage(m_Indices.size() * sizeof(GLuint), m_Indices.data(), GL_NONE);
	m_Vao->AttachVertexBuffer(m_Vbo, sizeof(Vertex), 0);
	m_Vao->AttachIndexBuffer(m_Ibo);
	m_Vao->LinkAttribute(0, 0, 3, GL_FLOAT, (GLuint)(0 * sizeof(glm::vec3)));
	m_Vao->LinkAttribute(0, 1, 3, GL_FLOAT, (GLuint)(1 * sizeof(glm::vec3)));
	m_Vao->LinkAttribute(0, 2, 3, GL_FLOAT, (GLuint)(2 * sizeof(glm::vec3)));
	m_Vao->LinkAttribute(0, 3, 2, GL_FLOAT, (GLuint)(3 * sizeof(glm::vec3)));

	m_VertexCount = m_Vertices.size();
	m_IndexCount = m_Indices.size();

	/*
	m_Vertices.clear();
	m_Vertices.shrink_to_fit();
	m_Indices.clear();
	m_Indices.shrink_to_fit();
	*/
}

void Geometry::UpdateInstanceSsbo()
{
	if (m_Instances.size() != 0)
		m_InstanceSsbo->BufferData(m_Instances.size() * sizeof(glm::uvec4), m_Instances.data(), GL_DYNAMIC_DRAW);
}

void Geometry::UpdateShadowInstanceSsbo()
{
	if(m_ShadowInstances.size() != 0)
		m_ShadowInstanceSsbo->BufferData(m_ShadowInstances.size() * sizeof(GLuint), m_ShadowInstances.data(), GL_DYNAMIC_DRAW);
}

void Geometry::GenerateObb()
{
	glm::vec3 maxPosition{ std::numeric_limits<float>::lowest() };
	glm::vec3 minPosition{ std::numeric_limits<float>::max() };

	for (auto& position : m_Surfacepoints)
	{
		if (position.x > maxPosition.x) maxPosition.x = position.x;
		if (position.y > maxPosition.y) maxPosition.y = position.y;
		if (position.z > maxPosition.z) maxPosition.z = position.z;
		if (position.x < minPosition.x) minPosition.x = position.x;
		if (position.y < minPosition.y) minPosition.y = position.y;
		if (position.z < minPosition.z) minPosition.z = position.z;
	}

	m_Obb[0] = glm::vec3(maxPosition.x, maxPosition.y, maxPosition.z);
	m_Obb[1] = glm::vec3(maxPosition.x, maxPosition.y, minPosition.z);
	m_Obb[2] = glm::vec3(maxPosition.x, minPosition.y, maxPosition.z);
	m_Obb[3] = glm::vec3(maxPosition.x, minPosition.y, minPosition.z);
	m_Obb[4] = glm::vec3(minPosition.x, maxPosition.y, maxPosition.z);
	m_Obb[5] = glm::vec3(minPosition.x, maxPosition.y, minPosition.z);
	m_Obb[6] = glm::vec3(minPosition.x, minPosition.y, maxPosition.z);
	m_Obb[7] = glm::vec3(minPosition.x, minPosition.y, minPosition.z);

	m_ObbMax = maxPosition;
	m_ObbMin = minPosition;
	m_ObbOrigin = 0.5f * (minPosition + maxPosition);
	m_ObbExtents = 0.5f * (maxPosition - minPosition);
}

void Geometry::GenerateTangents()
{
	for (int i = 0; i < m_Indices.size(); i += 3)
	{
		auto& vertex1 = m_Vertices[m_Indices[i]];
		auto& vertex2 = m_Vertices[m_Indices[i + 1]];
		auto& vertex3 = m_Vertices[m_Indices[i + 2]];

		vertex1.normal = glm::vec3(0);
		vertex2.normal = glm::vec3(0);
		vertex3.normal = glm::vec3(0);

		vertex1.tangent = glm::vec3(0);
		vertex2.tangent = glm::vec3(0);
		vertex3.tangent = glm::vec3(0);
	}

	for (int i = 0; i < m_Indices.size(); i += 3)
	{
		auto& vertex1 = m_Vertices[m_Indices[i]];
		auto& vertex2 = m_Vertices[m_Indices[i+1]];
		auto& vertex3 = m_Vertices[m_Indices[i+2]];

		glm::vec3 edge1 = vertex2.position - vertex1.position;
		glm::vec3 edge2 = vertex3.position - vertex1.position;

		glm::vec2 deltaUV1 = vertex2.texcoord - vertex1.texcoord;
		glm::vec2 deltaUV2 = vertex3.texcoord - vertex1.texcoord;

		float det = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent;
		tangent.x = det * (edge1.x * deltaUV2.y - edge2.x * deltaUV1.y);
		tangent.y = det * (edge1.y * deltaUV2.y - edge2.y * deltaUV1.y);
		tangent.z = det * (edge1.z * deltaUV2.y - edge2.z * deltaUV1.y);
	
		glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
		tangent = glm::normalize(tangent);

		vertex1.normal += normal;
		vertex2.normal += normal;
		vertex3.normal += normal;

		vertex1.tangent += tangent;
		vertex2.tangent += tangent;
		vertex3.tangent += tangent;
	}
}