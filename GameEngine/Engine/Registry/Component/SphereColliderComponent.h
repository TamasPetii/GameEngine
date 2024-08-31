#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <array>
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

using namespace physx;

struct ENGINE_API SphereColliderComponent : public Component
{
	bool calculateAutomatic = true;
	float radius;
	PxSphereGeometry sphereGeometry;
};