#pragma once
#include "EngineApi.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <Render/OpenGL/TextureGL.h>

struct ENGINE_API SpotLightComponent
{
	bool toRender;
	glm::vec3 color{ 1.f };
	glm::vec3 position{ 0, 1, 0 };
	glm::vec3 direction{ -1, -1, -1 };
	glm::vec4 angles{ 5.f, 25.f, 0, 0 };
	float length = 25.f;
	float strength = 1.f;
	int updateFrequency = 2;
	int shadowSize = 2048;
	bool useShadow{ false };
	float farPlane;
	glm::mat4 viewProj;
	glm::mat4 proxyTransform;
	std::vector<char> visibleEntities;
	std::shared_ptr<FramebufferGL> frameBuffer;
};

struct ENGINE_API SpotLightGLSL
{
	SpotLightGLSL(const SpotLightComponent& component)
	{
		this->color = glm::vec4(component.color, component.strength);
		this->position = glm::vec4(component.position, component.useShadow ? 1 : 0);
		this->direction = glm::vec4(component.direction, component.farPlane);
		this->viewProj = component.viewProj;
		this->angles = component.angles;
		this->shadowTexture = component.frameBuffer->GetTextureHandler("depth");
	}

	glm::vec4 color; //3 float color + 1 float strength
	glm::vec4 position; //3 float position + 1 float use shadow
	glm::vec4 direction; //3 float direction + 1 float farplane
	glm::mat4 viewProj;
	glm::vec4 angles; 
	GLuint64 shadowTexture;
	glm::vec2 filler;
};