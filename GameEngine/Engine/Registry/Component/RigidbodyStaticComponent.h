#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

using namespace physx;

class ENGINE_API RigidbodyStaticComponent : public Component
{
public:
	~RigidbodyStaticComponent()
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

		if (staticActor)
		{
			staticActor->release();
			staticActor = nullptr;
		}
	}

	float sFriction = 1.f;
	float dFriction = 1.f;
	float restitution = 0.1f;
	PxShape* shape = nullptr;
	PxMaterial* material = nullptr;
	PxRigidStatic* staticActor = nullptr;
};