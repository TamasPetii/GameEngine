#include "Cylinder.h"

Cylinder::Cylinder()
{
	m_Name = "Cylinder";
	m_Count = 50;
	m_RadiusTop = 1.f;
	m_RadiusBottom = 1.f;
	m_Height = 1.f;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GenerateShape(vertices, indices);

	UploadToGpu(vertices, indices);
}

void Cylinder::GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
	std::vector<glm::vec3> topPositions;
	std::vector<glm::vec3> bottomPositions;

	for (int p = 0; p <= m_Count; p++)
	{
		float u = p / float(m_Count);
		float theta = u * (2 * M_PI);
		topPositions.push_back(glm::vec3(m_RadiusTop * sinf(theta), m_Height, m_RadiusTop * cosf(theta)));
		bottomPositions.push_back(glm::vec3(m_RadiusBottom * sinf(theta), -m_Height, m_RadiusBottom * cosf(theta)));
	}

	#pragma region Top Circle Vertex and Index
	{
		auto TB = Shape::GenerateTB(glm::vec3(0, 1, 0));

		Vertex topMiddleVertex;
		topMiddleVertex.position = glm::vec3(0, m_Height, 0);
		topMiddleVertex.normal = glm::vec3(0, 1, 0);
		topMiddleVertex.tangent = glm::normalize(TB.first);
		topMiddleVertex.bitangent = glm::normalize(TB.second);
		topMiddleVertex.texture = glm::vec2(0.5, 0.5);
		vertices.push_back(topMiddleVertex);

		for (int p = 0; p <= m_Count; p++)
		{
			float u = p / float(m_Count);
			float theta = u * (2 * M_PI);

			Vertex vertex;
			vertex.position = topPositions[p];
			vertex.normal = glm::vec3(0, 1, 0);
			vertex.tangent = glm::normalize(TB.first);
			vertex.bitangent = glm::normalize(TB.second);
			vertex.texture = glm::vec2((cosf(theta) + 1) * 0.5f, (sinf(theta) + 1) * 0.5f);
			vertices.push_back(vertex);
		}

		int start = 0;
		for (int p = 0; p < m_Count; p++)
		{
			indices.push_back(start);
			indices.push_back(start + p + 1);
			indices.push_back(start + p + 2);
		}
	}
	#pragma endregion

	#pragma region Bottom Circle Vertex and Index
	{
		auto TB = Shape::GenerateTB(glm::vec3(0, -1, 0));

		Vertex bottomMiddleVertex;
		bottomMiddleVertex.position = glm::vec3(0, -m_Height, 0);
		bottomMiddleVertex.normal = glm::vec3(0, -1, 0);
		bottomMiddleVertex.tangent = glm::normalize(TB.first);
		bottomMiddleVertex.bitangent = glm::normalize(TB.second);
		bottomMiddleVertex.texture = glm::vec2(0.5, 0.5);
		vertices.push_back(bottomMiddleVertex);

		for (int p = 0; p <= m_Count; p++)
		{
			float u = p / float(m_Count);
			float theta = u * (2 * M_PI);

			Vertex vertex;
			vertex.position = bottomPositions[p];
			vertex.normal = glm::vec3(0, -1, 0);
			vertex.tangent = glm::normalize(TB.first);
			vertex.bitangent = glm::normalize(TB.second);
			vertex.texture = glm::vec2((sinf(theta) + 1) * 0.5f, (cosf(theta) + 1) * 0.5f);
			vertices.push_back(vertex);
		}

		int start = 1 + m_Count + 1;
		for (int p = 0; p < m_Count; p++)
		{
			indices.push_back(start);
			indices.push_back(start + p + 2);
			indices.push_back(start + p + 1);
		}
	}
	#pragma endregion

	#pragma region Wall Vertex and Index
	{
		for (int p = 0; p <= m_Count; p++)
		{
			float u = p / float(m_Count);
			float theta = u * (2 * M_PI);

			auto normal = glm::vec3(sinf(theta), 0, cosf(theta));
			auto TB = Shape::GenerateTB(normal);

			Vertex topVertex;
			topVertex.position = topPositions[p];
			topVertex.normal = glm::normalize(normal);
			topVertex.tangent = glm::normalize(TB.first);
			topVertex.bitangent = glm::normalize(TB.second);
			topVertex.texture = glm::vec2(1 - u, 1);
			vertices.push_back(topVertex);

			Vertex bottomVertex;
			bottomVertex.position = bottomPositions[p];
			bottomVertex.normal = glm::normalize(normal);
			bottomVertex.tangent = glm::normalize(TB.first);
			bottomVertex.bitangent = glm::normalize(TB.second);
			bottomVertex.texture = glm::vec2(1 - u, 0);
			vertices.push_back(bottomVertex);
		}

		int index = 0;
		int start = 2 * (1 + m_Count + 1);
		for (int p = 0; p < m_Count; p++)
		{
			int c = start + index;
			indices.push_back(c);
			indices.push_back(c + 1);
			indices.push_back(c + 3);

			indices.push_back(c);
			indices.push_back(c + 3);
			indices.push_back(c + 2);

			index += 2;
		}
	}
	#pragma endregion
}


void Cylinder::ReGenerate()
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GenerateShapeAverage(vertices, indices);

	m_VertexCount = vertices.size();
	m_IndexCount = indices.size();
	m_Vbo->AttachData(vertices, GL_STATIC_DRAW);
	m_Ibo->AttachData(indices, GL_STATIC_DRAW);
}

void Cylinder::GenerateShapeAverage(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
	std::vector<glm::vec3> topPositions;
	std::vector<glm::vec3> bottomPositions;

	for (int p = 0; p <= m_Count; p++)
	{
		float u = p / float(m_Count);
		float theta = u * (2 * M_PI);
		topPositions.push_back(glm::vec3(m_RadiusTop * sinf(theta), m_Height, m_RadiusTop * cosf(theta)));
		bottomPositions.push_back(glm::vec3(m_RadiusBottom * sinf(theta), -m_Height, m_RadiusBottom * cosf(theta)));
	}

	#pragma region Top Circle Vertex and Index
	{
		auto TB = Shape::GenerateTB(glm::vec3(0, 1, 0));

		Vertex topMiddleVertex;
		topMiddleVertex.position = glm::vec3(0, m_Height, 0);
		topMiddleVertex.normal = glm::vec3(0, 1, 0);
		topMiddleVertex.tangent = glm::normalize(TB.first);
		topMiddleVertex.bitangent = glm::normalize(TB.second);
		topMiddleVertex.texture = glm::vec2(0.5, 0.5);
		vertices.push_back(topMiddleVertex);

		for (int p = 0; p <= m_Count; p++)
		{
			float u = p / float(m_Count);
			float theta = u * (2 * M_PI);

			Vertex vertex;
			vertex.position = topPositions[p];
			vertex.normal = glm::vec3(0, 1, 0);
			vertex.tangent = glm::normalize(TB.first);
			vertex.bitangent = glm::normalize(TB.second);
			vertex.texture = glm::vec2((cosf(theta) + 1) * 0.5f, (sinf(theta) + 1) * 0.5f);
			vertices.push_back(vertex);
		}

		int start = 0;
		for (int p = 0; p < m_Count; p++)
		{
			indices.push_back(start);
			indices.push_back(start + p + 1);
			indices.push_back(start + p + 2);
		}
	}
#pragma endregion

#pragma region Bottom Circle Vertex and Index
	{
		auto TB = Shape::GenerateTB(glm::vec3(0, -1, 0));

		Vertex bottomMiddleVertex;
		bottomMiddleVertex.position = glm::vec3(0, -m_Height, 0);
		bottomMiddleVertex.normal = glm::vec3(0, -1, 0);
		bottomMiddleVertex.tangent = glm::normalize(TB.first);
		bottomMiddleVertex.bitangent = glm::normalize(TB.second);
		bottomMiddleVertex.texture = glm::vec2(0.5, 0.5);
		vertices.push_back(bottomMiddleVertex);

		for (int p = 0; p <= m_Count; p++)
		{
			float u = p / float(m_Count);
			float theta = u * (2 * M_PI);

			Vertex vertex;
			vertex.position = bottomPositions[p];
			vertex.normal = glm::vec3(0, -1, 0);
			vertex.tangent = glm::normalize(TB.first);
			vertex.bitangent = glm::normalize(TB.second);
			vertex.texture = glm::vec2((sinf(theta) + 1) * 0.5f, (cosf(theta) + 1) * 0.5f);
			vertices.push_back(vertex);
		}

		int start = 1 + m_Count + 1;
		for (int p = 0; p < m_Count; p++)
		{
			indices.push_back(start);
			indices.push_back(start + p + 2);
			indices.push_back(start + p + 1);
		}
	}
#pragma endregion

#pragma region Wall Vertex and Index
	{
		for (int p = 0; p <= m_Count; p++)
		{
			float u = p / float(m_Count);
			float theta = u * (2 * M_PI);

			auto normal = glm::vec3(sinf(theta), 0, cosf(theta));
			auto TB = Shape::GenerateTB(normal);

			Vertex topVertex;
			topVertex.position = topPositions[p];
			topVertex.texture = glm::vec2(1 - u, 1);
			vertices.push_back(topVertex);

			Vertex bottomVertex;
			bottomVertex.position = bottomPositions[p];
			bottomVertex.texture = glm::vec2(1 - u, 0);
			vertices.push_back(bottomVertex);
		}

		int index = 0;
		int start = 2 * (1 + m_Count + 1);
		for (int p = 0; p < m_Count; p++)
		{
			int c = start + index;
			int ind0 = c;
			int ind1 = c + 1;
			int ind2 = c + 3;
			int ind3 = c + 2;

			indices.push_back(ind0);
			indices.push_back(ind1);
			indices.push_back(ind2);

			auto pos0 = std::make_tuple(vertices[ind0].position, vertices[ind1].position, vertices[ind2].position);
			auto tex0 = std::make_tuple(vertices[ind0].texture, vertices[ind1].texture, vertices[ind2].texture);
			auto tbn0 = Shape::GenerateTBN(pos0, tex0);

			averages[vertices[ind0].position].tangent += std::get<0>(tbn0);
			averages[vertices[ind0].position].bitangent += std::get<1>(tbn0);
			averages[vertices[ind0].position].normal += std::get<2>(tbn0);

			averages[vertices[ind1].position].tangent += std::get<0>(tbn0);
			averages[vertices[ind1].position].bitangent += std::get<1>(tbn0);
			averages[vertices[ind1].position].normal += std::get<2>(tbn0);

			averages[vertices[ind2].position].tangent += std::get<0>(tbn0);
			averages[vertices[ind2].position].bitangent += std::get<1>(tbn0);
			averages[vertices[ind2].position].normal += std::get<2>(tbn0);

			indices.push_back(ind0);
			indices.push_back(ind2);
			indices.push_back(ind3);

			auto pos1 = std::make_tuple(vertices[ind0].position, vertices[ind2].position, vertices[ind3].position);
			auto tex1 = std::make_tuple(vertices[ind0].texture, vertices[ind2].texture, vertices[ind3].texture);
			auto tbn1 = Shape::GenerateTBN(pos1, tex1);

			averages[vertices[ind0].position].tangent += std::get<0>(tbn1);
			averages[vertices[ind0].position].bitangent += std::get<1>(tbn1);
			averages[vertices[ind0].position].normal += std::get<2>(tbn1);

			averages[vertices[ind2].position].tangent += std::get<0>(tbn1);
			averages[vertices[ind2].position].bitangent += std::get<1>(tbn1);
			averages[vertices[ind2].position].normal += std::get<2>(tbn1);

			averages[vertices[ind3].position].tangent += std::get<0>(tbn1);
			averages[vertices[ind3].position].bitangent += std::get<1>(tbn1);
			averages[vertices[ind3].position].normal += std::get<2>(tbn1);

			index += 2;
		}
	}

	for (int start = 2 * (1 + m_Count + 1); start < vertices.size(); start++)
	{
		auto vectors = averages[vertices[start].position];
		vertices[start].normal = glm::normalize(vectors.normal);
		vertices[start].tangent = glm::normalize(vectors.tangent);
		vertices[start].bitangent = glm::normalize(vectors.bitangent);
	}
#pragma endregion

	averages.clear();
}