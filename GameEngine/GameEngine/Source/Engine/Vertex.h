#pragma once
#include <GLM/glm.hpp>
#include <iostream>

struct Vertex
{
	glm::vec3 mPosition;
	glm::vec3 mNormal;
	glm::vec2 mTexture;

	friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex);
};
