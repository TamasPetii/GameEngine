#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <array>
#include <glm/glm.hpp>

using namespace physx;

struct ENGINE_API SphereColliderComponent
{
	bool isTrigger = false;
	bool calculateAutomatic = true;
	bool bindToTransformTranslate = true;
	bool bindToTransformScale = true;

	float radius = 1.f;
	glm::vec3 origin;
	float transformedRadius;
	glm::vec3 transformedOrigin;
	PxSphereGeometry sphereGeometry;
};