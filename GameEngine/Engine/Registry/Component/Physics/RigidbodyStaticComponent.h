#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>
#include <memory>

using namespace physx;

struct ENGINE_API RigidbodyStaticComponent
{
	RigidbodyStaticComponent();

	float sFriction;
	float dFriction;
	float restitution;
	std::shared_ptr<PxShape> shape;
	std::shared_ptr<PxMaterial> material;
	std::shared_ptr<PxRigidStatic> staticActor;
};