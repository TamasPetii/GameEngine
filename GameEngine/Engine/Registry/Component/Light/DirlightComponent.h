#pragma once
#include "EngineApi.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <Render/OpenGL/TextureGL.h>

struct ENGINE_API DirlightComponent
{
	glm::vec3 color{ 1.f };
	glm::vec3 direction{ -1.f, -0.9f, -0.8f };
	float strength = 0.5f; 
	int updateFrequency = 1;
	int shadowSize = 2048;
	bool useShadow{ false };
	float farPlane[5] = { 0.1f, 25.f, 70.f, 135.f, 250.f };
	glm::mat4 viewProj[4];
	std::shared_ptr<FramebufferGL> frameBuffer;
};

struct ENGINE_API DirlightGLSL
{
	DirlightGLSL(const DirlightComponent& component)
	{
		this->color = glm::vec4(component.color, component.strength);
		this->direction = glm::vec4(component.direction, component.useShadow ? 1 : 0);
		this->farPlane.x = component.farPlane[1];
		this->farPlane.y = component.farPlane[2];
		this->farPlane.z = component.farPlane[3];
		this->farPlane.w = component.farPlane[4];
		this->viewProj[0] = component.viewProj[0];
		this->viewProj[1] = component.viewProj[1];
		this->viewProj[2] = component.viewProj[2];
		this->viewProj[3] = component.viewProj[3];
		this->shadowTexture = component.frameBuffer->GetTextureHandler("depth");
		this->filler = 0;
	}

	glm::vec4 color; //3 float color + 1 float strength
	glm::vec4 direction; //3 float direction + 1 float use shadow
	glm::vec4 farPlane;
	glm::mat4 viewProj[4];
	GLuint64 shadowTexture;
	GLuint64 filler;
};

struct ENGINE_API DirlightLineGLSL
{
	DirlightLineGLSL(const glm::vec4& position, const glm::vec4& direction)
	{
		this->position = position;
		this->direction = direction;
	}

	glm::vec4 position; 
	glm::vec4 direction;
};