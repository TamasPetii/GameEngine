#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	GLuint index;
};

