#include "Rigidbody.h"

physx::PxScene* Rigidbody::mScene;
physx::PxPhysics* Rigidbody::mPhysics;

Rigidbody::Rigidbody(const physx::PxTransform& transform)
{
	mTransform = transform;
	mGeometry = physx::PxBoxGeometry(1, 1, 1);
	mMaterial = mPhysics->createMaterial(0.1, 0.1, 0.1);
	mShape = mPhysics->createShape(mGeometry, *mMaterial);
	mRigidBody = mPhysics->createRigidDynamic(mTransform);
	mRigidBody->attachShape(*mShape);
	mScene->addActor(*mRigidBody);
}