#pragma once
#include "EngineApi.h"
#include <array>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Registry/Entity.h"

class FramebufferGL;

struct ENGINE_API PointLightComponent
{
	PointLightComponent();
	virtual ~PointLightComponent();

	bool toRender;
	glm::vec3 color;
	glm::vec3 position;
	float strength;
	float weakenDistance;
	int updateFrequency;
	int shadowSize;
	bool useShadow;
	float farPlane;
	std::array<glm::mat4, 6> viewProj;
	std::vector<char> visibleEntities;
	std::shared_ptr<FramebufferGL> frameBuffer;
};

struct ENGINE_API PointLightGLSL
{
	PointLightGLSL(const PointLightComponent& component);

	glm::vec4 color; //3 float color + 1 float strength
	glm::vec4 position; //3 float position + 1 float use shadow
	glm::mat4 viewProj[6];
	glm::vec2 farPlane; //1 float farplane + 1 float filler
	glm::uint64 shadowTexture;
};