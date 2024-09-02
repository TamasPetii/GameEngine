#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

using namespace physx;

class ENGINE_API RigidbodyDynamicComponent : public Component
{
public:
	~RigidbodyDynamicComponent()
	{
		if (shape)
		{
			shape->release();
			shape = nullptr;
		}

		if (material)
		{
			material->release();
			material = nullptr;
		}

		if (dynamicActor)
		{
			dynamicActor->release();
			dynamicActor = nullptr;
		}
	}

	float mass = 1.f;
	float sFriction = 1.f;
	float dFriction = 1.f;
	float restitution = 0.1f;
	PxShape* shape = nullptr;
	PxMaterial* material = nullptr;
	PxRigidDynamic* dynamicActor = nullptr;
};