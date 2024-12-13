#pragma once
#include "EngineApi.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

class Animation;
class ShaderStorageBufferGL;

struct ENGINE_API AnimationComponent
{
	AnimationComponent();
	virtual ~AnimationComponent();

	bool lockToFps;
	float time;
	float speed;
	std::shared_ptr<Animation> animation;
	std::vector<glm::mat4> boneTransforms;
	std::shared_ptr<ShaderStorageBufferGL> boneTransformSsbo;
};