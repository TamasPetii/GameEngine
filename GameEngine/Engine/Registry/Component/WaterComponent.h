#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>
#include <Render/OpenGL/FramebufferGL.h>
#include <Render/OpenGL/TextureGL.h>

class ENGINE_API WaterComponent
{
public:
	int updateFrequency = 3;
	int size = 1024;
	glm::vec4 plane{ 0, 1, 0, 0 }; // x,y,z = normal | w = height
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
		dudv = component.dudv != nullptr ? component.dudv->GetTextureHandler() : 0;
	}

	glm::vec4 plane;
	glm::vec4 scale;
	glm::vec2 move;
	GLuint64 dudv;
};