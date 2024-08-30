#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <array>
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

using namespace physx;

struct ENGINE_API BoxColliderComponent : public Component
{
	bool calculateAutomatic = false;
	glm::vec3 halfExtents;
	PxBoxGeometry boxGeometry;
};