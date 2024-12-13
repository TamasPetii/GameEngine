#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>
#include <memory>

class TextureGL;

struct ENGINE_API MaterialComponent
{
	MaterialComponent();

	bool useBloom;
	glm::vec4 color;
	float shinniness;
	glm::vec2 textureScale;
	std::shared_ptr<TextureGL> diffuse;
	std::shared_ptr<TextureGL> normal;
	std::shared_ptr<TextureGL> specular;
};

struct ENGINE_API MaterialGLSL
{
	MaterialGLSL(const MaterialComponent& component);

	glm::vec4 color;
	glm::vec4 shinniness;
	glm::uint64 diffuse;
	glm::uint64 normal;
	glm::uint64 specular;
	glm::uint64 filler;
};