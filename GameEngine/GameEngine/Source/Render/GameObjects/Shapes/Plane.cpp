#include "Plane.h"

Plane::Plane() : Shape()
{
	mName = "Plane";
	CreateVertices();
	CreateIndices();
	UploadBuffers();
}

void Plane::CreateVertices()
{
	mVertices.clear();
	mVertices = std::vector<Vertex>
	{
		{ glm::vec3(-1, 0, -1), glm::vec3(0, 1, 0), glm::vec2(0, 1) },
		{ glm::vec3(-1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(0, 0) },
		{ glm::vec3(1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(1, 0) },
		{ glm::vec3(1, 0, -1), glm::vec3(0, 1, 0), glm::vec2(1, 1) },
	};
}

void Plane::CreateIndices()
{
	mIndices.clear();
	mIndices.push_back(0);
	mIndices.push_back(1);
	mIndices.push_back(2);
	mIndices.push_back(0);
	mIndices.push_back(2);
	mIndices.push_back(3);
}