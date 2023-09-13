#include "Vertex.h"

Vertex::Vertex() :
	position(glm::vec3(0)),
	normal(glm::vec3(0)),
	tangent(glm::vec3(0)),
	bitangent(glm::vec3(0)),
	texture(glm::vec3(0))
{
}

Vertex::Vertex(const glm::vec3& position, const glm::vec2& texture) :
	position(position),
	normal(glm::vec3(0)),
	tangent(glm::vec3(0)),
	bitangent(glm::vec3(0)),
	texture(texture)
{
}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texture) :
	position(position),
	normal(normal),
	tangent(glm::vec3(0)),
	bitangent(glm::vec3(0)),
	texture(texture)
{
}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& bitangent, const glm::vec2& texture) :
	position(position),
	normal(normal),
	tangent(tangent),
	bitangent(bitangent),
	texture(texture)
{
}