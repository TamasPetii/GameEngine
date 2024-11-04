#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <glm/glm.hpp>

using namespace physx;

class ENGINE_API RigidbodyStaticComponent
{
public:
	float sFriction = 1.f;
	float dFriction = 1.f;
	float restitution = 0.1f;

	std::shared_ptr<PxShape> shape = nullptr;
	std::shared_ptr<PxMaterial> material = nullptr;
	std::shared_ptr<PxRigidStatic> staticActor = nullptr;
};