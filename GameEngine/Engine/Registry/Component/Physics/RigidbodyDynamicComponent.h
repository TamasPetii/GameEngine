#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>
#include <memory>

using namespace physx;

struct ENGINE_API RigidbodyDynamicComponent
{
	RigidbodyDynamicComponent();

	float mass;
	float sFriction;
	float dFriction;
	float restitution;
	bool isKinematic;
	bool disableGravity;
	bool lockRotation[3];
	bool lockPosition[3];
	std::shared_ptr<PxShape> shape;
	std::shared_ptr<PxMaterial> material;
	std::shared_ptr<PxRigidDynamic> dynamicActor;
};