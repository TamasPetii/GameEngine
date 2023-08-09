#include "Cube.h"

Cube::Cube() : Shape()
{
	mName = "Cube";
	CreateVertices();
	CreateIndices();
	UploadBuffers();
}

void Cube::CreateVertices()
{
	mVertices.clear();
	//Front
	mVertices = std::vector<Vertex>
	{

		{ glm::vec3(-1, 1, 1), glm::vec3(0, 0, 1), glm::vec2(0, 1) },
		{ glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0) },
		{ glm::vec3(1, -1, 1), glm::vec3(0, 0, 1), glm::vec2(1, 0) },
		{ glm::vec3(1, 1, 1), glm::vec3(0, 0, 1), glm::vec2(1, 1) },

		{ glm::vec3(1, 1, -1), glm::vec3(0, 0, -1), glm::vec2(0, 1) },
		{ glm::vec3(1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(0, 0) },
		{ glm::vec3(-1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(1, 0) },
		{ glm::vec3(-1, 1, -1), glm::vec3(0, 0, -1), glm::vec2(1, 1) },

		{ glm::vec3(-1, 1, -1), glm::vec3(-1, 0, 0), glm::vec2(0, 1) },
		{ glm::vec3(-1, -1, -1), glm::vec3(-1, 0, 0), glm::vec2(0, 0) },
		{ glm::vec3(-1, -1, 1), glm::vec3(-1, 0, 0), glm::vec2(1, 0) },
		{ glm::vec3(-1, 1, 1), glm::vec3(-1, 0, 0), glm::vec2(1, 1) },

		{ glm::vec3(1, 1, 1), glm::vec3(1, 0, 0), glm::vec2(0, 1) },
		{ glm::vec3(1, -1, 1), glm::vec3(1, 0, 0), glm::vec2(0, 0) },
		{ glm::vec3(1, -1, -1), glm::vec3(1, 0, 0), glm::vec2(1, 0) },
		{ glm::vec3(1, 1, -1), glm::vec3(1, 0, 0), glm::vec2(1, 1) },

		{ glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0), glm::vec2(0, 1) },
		{ glm::vec3(-1, 1, 1), glm::vec3(0, 1, 0), glm::vec2(0, 0) },
		{ glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), glm::vec2(1, 0) },
		{ glm::vec3(1, 1, -1), glm::vec3(0, 1, 0), glm::vec2(1, 1) },

		{ glm::vec3(-1, -1, 1), glm::vec3(0, -1, 0), glm::vec2(0, 1) },
		{ glm::vec3(-1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(0, 0) },
		{ glm::vec3(1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(1, 0) },
		{ glm::vec3(1, -1, 1), glm::vec3(0, -1, 0), glm::vec2(1, 1) }
	};
}

void Cube::CreateIndices()
{
	mIndices.clear();
	for (unsigned int i = 0; i < 24; i += 4)
	{
		mIndices.push_back(0 + i);
		mIndices.push_back(1 + i);
		mIndices.push_back(2 + i);
		mIndices.push_back(0 + i);
		mIndices.push_back(2 + i);
		mIndices.push_back(3 + i);
	}
}