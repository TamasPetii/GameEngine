#pragma once
#include "EngineApi.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <Registry/Component/Component.h>
#include <Render/OpenGL/TextureGL.h>
#include <Registry/Entity.h>
#include <unordered_set>

struct ENGINE_API PointLightComponent : public Component
{
	glm::vec3 color{ 1.f };
	glm::vec3 position{ 0, 1, 0 };
	float strength = 1.f;
	int updateFrequency = 1;
	int shadowSize = 2048;
	bool useShadow{ false };
	float farPlane;
	glm::mat4 viewProj[6];
	GLuint shadowFramebuffer;
	GLuint shadowTexture;
	GLuint64 shadowTextureHandler;
	std::unordered_set<Entity> visibleEntities;
};

struct ENGINE_API PointLightGLSL
{
	PointLightGLSL(const PointLightComponent& component)
	{
		this->color = glm::vec4(component.color, component.strength);
		this->position = glm::vec4(component.position, component.useShadow ? 1 : 0);
		this->viewProj[0] = component.viewProj[0];
		this->viewProj[1] = component.viewProj[1];
		this->viewProj[2] = component.viewProj[2];
		this->viewProj[3] = component.viewProj[3];
		this->viewProj[4] = component.viewProj[4];
		this->viewProj[5] = component.viewProj[5];
		this->farPlane = glm::vec2(component.farPlane, 0);
		this->shadowTexture = component.shadowTextureHandler;
	}

	glm::vec4 color; //3 float color + 1 float strength
	glm::vec4 position; //3 float position + 1 float use shadow
	glm::mat4 viewProj[6];
	glm::vec2 farPlane; //1 float farplane + 1 float filler
	GLuint64 shadowTexture;
};