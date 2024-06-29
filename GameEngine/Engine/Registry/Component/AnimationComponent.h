#pragma once
#include "EngineApi.h"
#include <memory>
#include <glm/glm.hpp>
#include "Animation/Animation.h"
#include "Registry/Component/Component.h"

class ENGINE_API AnimationComponent : public Component
{
public:
	float time;
	std::shared_ptr<Animation> animation;
	std::vector<glm::mat4> boneTransforms;
	std::shared_ptr<ShaderStorageBufferGL> boneTransformSsbo;
};