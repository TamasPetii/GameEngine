#pragma once
#include "EngineApi.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

class FramebufferGL;

struct ENGINE_API SpotLightComponent
{
	SpotLightComponent();
	virtual ~SpotLightComponent();

	bool toRender;
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec4 angles;
	float length;
	float strength;
	int updateFrequency;
	int shadowSize;
	bool useShadow;
	float farPlane;
	glm::mat4 viewProj;
	glm::mat4 proxyTransform;
	std::vector<char> visibleEntities;
	std::shared_ptr<FramebufferGL> frameBuffer;
};

struct ENGINE_API SpotLightGLSL
{
	SpotLightGLSL(const SpotLightComponent& component);

	glm::vec4 color; //3 float color + 1 float strength
	glm::vec4 position; //3 float position + 1 float use shadow
	glm::vec4 direction; //3 float direction + 1 float farplane
	glm::mat4 viewProj;
	glm::vec4 angles; 
	glm::uint64 shadowTexture;
	glm::uint64 filler;
};