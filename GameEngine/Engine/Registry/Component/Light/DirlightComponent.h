#pragma once
#include "EngineApi.h"
#include <array>
#include <memory>
#include <glm/glm.hpp>

class FramebufferGL;

struct ENGINE_API DirlightComponent
{
	DirlightComponent();

	glm::vec3 color;
	glm::vec3 direction;
	float strength; 
	int updateFrequency;
	int shadowSize;
	bool useShadow;
	std::array<float, 5> farPlane;
	std::array<glm::mat4, 4> viewProj;
	std::shared_ptr<FramebufferGL> frameBuffer;
};

struct ENGINE_API DirlightGLSL
{
	DirlightGLSL(const DirlightComponent& component);

	glm::vec4 color; //3 float color + 1 float strength
	glm::vec4 direction; //3 float direction + 1 float use shadow
	glm::vec4 farPlane;
	glm::mat4 viewProj[4];
	glm::uint64 shadowTexture;
	glm::uint64 filler;
};

struct ENGINE_API DirlightLineGLSL
{
	DirlightLineGLSL(const glm::vec4& position, const glm::vec4& direction);

	glm::vec4 position; 
	glm::vec4 direction;
};