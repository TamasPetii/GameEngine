#include "Cube.h"

Cube::Cube() : GameObject()
{
	CreateBuffers();
	UploadBuffers();
}

Cube::~Cube()
{
	delete mVao;
	delete mIbo;
	delete mVbo;
}

void Cube::CreateBuffers()
{
	mVao = new VertexArrayObject();
	mIbo = new IndexBufferObject();
	mVbo = new VertexBufferObject();
}

void Cube::UploadBuffers()
{
	std::vector<Vertex> vertices;

	//Front
	vertices.push_back({ glm::vec3(-1, 1, -1), glm::vec3(0, 0, -1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(1, 1, -1), glm::vec3(0, 0, -1), glm::vec2(1, 1) });

	//Back
	vertices.push_back({ glm::vec3(1, 1, 1), glm::vec3(0, 0, 1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(1, -1, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-1, 1, 1), glm::vec3(0, 0, 1), glm::vec2(1, 1) });

	//Left
	vertices.push_back({ glm::vec3(-1, 1, 1), glm::vec3(-1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-1, -1, 1), glm::vec3(-1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-1, -1, -1), glm::vec3(-1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-1, 1, -1), glm::vec3(-1, 0, 0), glm::vec2(1, 1) });

	//Right
	vertices.push_back({ glm::vec3(1, 1, -1), glm::vec3(1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(1, -1, -1), glm::vec3(1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(1, -1, 1), glm::vec3(1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(1, 1, 1), glm::vec3(1, 0, 0), glm::vec2(1, 1) });

	//Top
	vertices.push_back({ glm::vec3(-1, 1, 1), glm::vec3(0, 1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(1, 1, -1), glm::vec3(0, 1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), glm::vec2(1, 1) });

	//Bottom
	vertices.push_back({ glm::vec3(-1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-1, -1, 1), glm::vec3(0, -1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(1, -1, 1), glm::vec3(0, -1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(1, 1) });

	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < 24; i += 4)
	{
		indices.push_back(0 + i);
		indices.push_back(2 + i);
		indices.push_back(1 + i);
		indices.push_back(2 + i);
		indices.push_back(0 + i);
		indices.push_back(3 + i);
	}

	mIndexCount = indices.size();
	mVertexCount = vertices.size();

	std::cout << mIndexCount << " " << mVertexCount << std::endl;

	mVao->Bind();
	mVbo->AttachData(vertices, GL_STATIC_DRAW);
	mIbo->AttachData(indices, GL_STATIC_DRAW);

	mVao->LinkAttribute(*mVbo, 0, 3, GL_FLOAT, sizeof(Vertex), nullptr);
	mVao->LinkAttribute(*mVbo, 1, 3, GL_FLOAT, sizeof(Vertex), (const void*)(1 * sizeof(glm::vec3)));
	mVao->LinkAttribute(*mVbo, 2, 2, GL_FLOAT, sizeof(Vertex), (const void*)(2 * sizeof(glm::vec3)));

	mVao->UnBind();
	mVbo->UnBind();
	mIbo->UnBind();
}

void Cube::Render()
{
	mVao->Bind();
	glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, nullptr);
	mVao->UnBind();
}