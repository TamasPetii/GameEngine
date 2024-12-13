#include "Vertex.h"

Vertex::Vertex(const glm::vec3& position, const glm::vec2& texcoord) : position(position), texcoord(texcoord) {}
Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texcoord) : position(position), normal(normal), texcoord(texcoord) {}
Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec2& texcoord) : position(position), normal(normal), tangent(tangent), texcoord(texcoord) {}
Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec2& texcoord, glm::uint index) : position(position), normal(normal), tangent(tangent), texcoord(texcoord), index(index) {}