#pragma once
#include "../Component.h"
#include <PHYSX/PxPhysicsAPI.h>
#include <PHYSX/geometry/PxBoxGeometry.h>

class Rigidbody : public Component
{
public:
	Rigidbody(const physx::PxTransform& transform);
	static void AttachScene(physx::PxScene* scene) { mScene = scene; }
	static void AttachPhysics(physx::PxPhysics* physics) { mPhysics = physics; }

	static physx::PxScene* mScene;
	static physx::PxPhysics* mPhysics;
	physx::PxMaterial* mMaterial;
	physx::PxRigidBody* mRigidBody;
	physx::PxShape* mShape;
	physx::PxBoxGeometry mGeometry;
	physx::PxTransform mTransform;
};

