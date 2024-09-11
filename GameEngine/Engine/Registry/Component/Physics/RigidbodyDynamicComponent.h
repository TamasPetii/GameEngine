#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <memory>
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

using namespace physx;

class ENGINE_API RigidbodyDynamicComponent : public Component
{
public:
	float mass = 1.f;
	float sFriction = 1.f;
	float dFriction = 1.f;
	float restitution = 0.1f;

	std::shared_ptr<PxShape> shape = nullptr;
	std::shared_ptr<PxMaterial> material = nullptr;
	std::shared_ptr<PxRigidDynamic> dynamicActor = nullptr;
};