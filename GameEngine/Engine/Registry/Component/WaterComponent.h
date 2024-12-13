#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>
#include <memory>

class FramebufferGL;
class TextureGL;

struct ENGINE_API WaterComponent
{
	WaterComponent();

	int updateFrequency;
	int size;
	glm::vec4 plane; // x,y,z = normal | w = height
	glm::vec2 dudvScale;
	glm::vec2 dudvWaveStrength;
	float dudvMoveFactor;
	float dudvMoveSpeed;
	std::shared_ptr<TextureGL> dudv;
	std::shared_ptr<FramebufferGL> reflectionFbo;
	std::shared_ptr<FramebufferGL> refractionFbo;
};

struct ENGINE_API WaterGLSL
{
	WaterGLSL(const WaterComponent& component);

	glm::vec4 plane;
	glm::vec4 scale;
	glm::vec2 move;
	glm::uint64 dudv;
};