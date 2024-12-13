#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>
#include <glm/glm.hpp>

using namespace physx;

struct ENGINE_API SphereColliderComponent
{
	SphereColliderComponent();

	bool isTrigger;
	bool calculateAutomatic;
	bool bindToTransformTranslate;
	bool bindToTransformScale;
	float radius;
	glm::vec3 origin;
	float transformedRadius;
	glm::vec3 transformedOrigin;
	PxSphereGeometry sphereGeometry;
};