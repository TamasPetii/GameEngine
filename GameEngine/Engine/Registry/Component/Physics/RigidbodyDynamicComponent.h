#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <memory>
#include <glm/glm.hpp>

using namespace physx;

class ENGINE_API RigidbodyDynamicComponent
{
public:
	float mass = 1.f;
	float sFriction = 1.f;
	float dFriction = 1.f;
	float restitution = 0.1f;
	bool isKinematic = false;
	bool disableGravity = false;
	bool lockRotation[3] = { false, false, false };
	bool lockPosition[3] = { false, false, false };

	std::shared_ptr<PxShape> shape = nullptr;
	std::shared_ptr<PxMaterial> material = nullptr;
	std::shared_ptr<PxRigidDynamic> dynamicActor = nullptr;
};