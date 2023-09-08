#pragma once
#include <GLM/glm.hpp>
#include <iostream>

struct Vertex
{
	Vertex();
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2 texture);
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& bitangent, const glm::vec2 texture);
	
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec2 texture;
};
