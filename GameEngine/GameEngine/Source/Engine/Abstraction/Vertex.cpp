#include "Vertex.h"

Vertex::Vertex() :
	position(glm::vec3(0)),
	normal(glm::vec3(0)),
	tangent(glm::vec3(0)),
	bitangent(glm::vec3(0)),
	texture(glm::vec3(0))
{
}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2 texture) :
	position(position),
	normal(normal),
	tangent(glm::vec3(0)),
	bitangent(glm::vec3(0)),
	texture(texture)
{
}

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& bitangent, const glm::vec2 texture) :
	position(position),
	normal(normal),
	tangent(tangent),
	bitangent(bitangent),
	texture(texture)
{
}

std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
{
	/*
	os << "________Vertex_______" << std::endl;
	os << "Position: { " << vertex.mPosition.x << ", " << vertex.mPosition.y << ", " << vertex.mPosition.z << " }" << std::endl;
	os << "Normal: { " << vertex.mNormal.x << ", " << vertex.mNormal.y << ", " << vertex.mNormal.z << " }" << std::endl;
	os << "Tex: { " << vertex.mTexture.x << ", " << vertex.mTexture.y << " }" << std::endl;
	*/
	return os;
}