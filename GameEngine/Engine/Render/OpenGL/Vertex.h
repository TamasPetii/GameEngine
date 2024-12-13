#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>

struct ENGINE_API Vertex
{
	Vertex() = default;
	Vertex(const glm::vec3& position, const glm::vec2& texcoord);
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texcoord);
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec2& texcoord);
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec2& texcoord, glm::uint index);
	
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 texcoord;
	glm::uint index;
};
