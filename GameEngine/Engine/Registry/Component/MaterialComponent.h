#pragma once
#include "EngineApi.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <Registry/Component/Component.h>
#include <Render/OpenGL/TextureGL.h>

struct ENGINE_API MaterialComponent : public Component
{
	bool useBloom{false};
	glm::vec4 color{1.f};
	float shinniness{0.25f};
	glm::vec2 textureScale{ 1.f, 1.f };
	std::shared_ptr<TextureGL> diffuse;
	std::shared_ptr<TextureGL> normal;
	std::shared_ptr<TextureGL> specular;
	std::shared_ptr<TextureGL> environment;
};

struct ENGINE_API MaterialGLSL
{
	MaterialGLSL(const MaterialComponent& component)
	{
		this->color = component.color;
		this->shinniness.x = component.shinniness;
		this->shinniness.y = component.useBloom ? 1 : 0;
		this->scale.x = component.textureScale.x;
		this->scale.y = component.textureScale.y;
		this->diffuse = component.diffuse ? component.diffuse->GetTextureHandler() : 0;
		this->normal = component.normal ? component.normal->GetTextureHandler() : 0;
		this->specular = component.specular ? component.specular->GetTextureHandler() : 0;
		this->environment = component.environment ? component.environment->GetTextureHandler() : 0;
	}

	glm::vec4 color;
	glm::vec4 shinniness;
	glm::vec4 scale;
	GLuint64 diffuse;
	GLuint64 normal;
	GLuint64 specular;
	GLuint64 environment;
};