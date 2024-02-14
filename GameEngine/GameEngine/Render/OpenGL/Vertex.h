#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Vertex
{
public:
	Vertex() = default;
	Vertex(const glm::vec3& position, const glm::vec2& texcoord) : position(position), texcoord(texcoord) {}
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texcoord) : position(position), normal(normal), texcoord(texcoord) {}
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec2& texcoord) : position(position), normal(normal), tangent(tangent), texcoord(texcoord) {}
	
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 texcoord;
};
