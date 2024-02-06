#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Vertex
{
public:
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texcoord) : position(position), normal(normal), texcoord(texcoord) {}
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
};
