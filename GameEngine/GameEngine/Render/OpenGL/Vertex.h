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
<<<<<<< HEAD
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec2& texcoord, unsigned int index) : position(position), normal(normal), tangent(tangent), texcoord(texcoord), index(index) {}
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 texcoord;
	GLuint index;
};
