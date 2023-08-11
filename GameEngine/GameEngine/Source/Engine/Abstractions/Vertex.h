#pragma once
#include <GLM/glm.hpp>
#include <iostream>

struct Vertex
{
	Vertex(const glm::vec3& position, const glm::vec2& texture) : mPosition(position), mTexture(texture) {}
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texture) : mPosition(position), mNormal(normal), mTexture(texture) {}

	glm::vec3 mPosition;
	glm::vec3 mNormal;
	glm::vec2 mTexture;

	friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex);
};
