#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

using namespace physx;

class ENGINE_API RigidbodyStaticComponent : public Component
{
public:
	float sFriction = 1.f;
	float dFriction = 1.f;
	float restitution = 0.1f;

	std::shared_ptr<PxShape> shape;
	std::shared_ptr<PxMaterial> material;
	std::shared_ptr<PxRigidStatic> staticActor;
};