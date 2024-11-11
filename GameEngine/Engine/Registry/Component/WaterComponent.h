#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>
#include <Render/OpenGL/FramebufferGL.h>
#include <Render/OpenGL/TextureGL.h>

class ENGINE_API WaterComponent
{
public:
	glm::vec4 plane{ 0, 1, 0, 0 }; // x,y,z = normal | w = height
	glm::ivec2 size{ 1024, 1024 };
	glm::vec2 dudvScale{ 1, 1 };
	glm::vec2 dudvWaveStrength{ 0.025, 0.025 };
	float dudvMoveFactor{0};
	float dudvMoveSpeed{0.030};
	std::shared_ptr<TextureGL> dudv = nullptr;
	std::shared_ptr<FramebufferGL> reflectionFbo = nullptr;
	std::shared_ptr<FramebufferGL> refractionFbo = nullptr;
};

class ENGINE_API WaterGLSL
{
public:
	WaterGLSL(const WaterComponent& component) 
	{
		plane = component.plane;
		scale = glm::vec4(component.dudvScale, component.dudvWaveStrength);
		move = glm::vec2(component.dudvMoveFactor, component.dudvMoveSpeed);

		if (component.dudv != nullptr)
			dudv = component.dudv->GetTextureHandler();
		else
			dudv = 0;
	}

	glm::vec4 plane;
	glm::vec4 scale;
	glm::vec2 move;
	GLuint64 dudv;
};