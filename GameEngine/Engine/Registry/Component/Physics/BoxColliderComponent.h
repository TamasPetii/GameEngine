#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>
#include <glm/glm.hpp>

using namespace physx;

struct ENGINE_API BoxColliderComponent
{
	BoxColliderComponent();

	bool isTrigger;
	bool calculateAutomatic;
	bool bindToTransformTranslate;
	bool bindToTransformScale;
	glm::vec3 halfExtents{1.f, 1.f, 1.f};
	glm::vec3 origin;
	glm::vec3 transformedExtents;
	glm::vec3 transformedOrigin;
	PxBoxGeometry boxGeometry;
};