#pragma once
#include "EngineApi.h"
#include <memory>
#include <glm/glm.hpp>
#include "Animation/Animation.h"

class ENGINE_API AnimationComponent
{
public:
	bool lockToFps = false;
	float time = 0;
	float speed = 1.f;
	std::vector<glm::mat4> boneTransforms;
	std::shared_ptr<Animation> animation = nullptr;
	std::shared_ptr<ShaderStorageBufferGL> boneTransformSsbo = nullptr;
};