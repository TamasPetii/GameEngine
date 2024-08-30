#pragma once

#include "EngineApi.h"
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

class ENGINE_API RigidbodyComponent : public Component
{
public:
	bool isStatic{false};
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;
	float mass;
};