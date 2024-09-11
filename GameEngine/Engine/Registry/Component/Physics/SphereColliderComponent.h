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
	float radius = 1.f;
	glm::vec3 origin;
	float transformedRadius;
	glm::vec3 transformedOrigin;
	PxSphereGeometry sphereGeometry;
};