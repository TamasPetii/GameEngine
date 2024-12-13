#pragma once
#include "EngineApi.h"
#include <memory>
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxSimulationEventCallback.h>

using namespace physx;

class Registry;

class ENGINE_API CollisionCallback : public PxSimulationEventCallback {
public:
    CollisionCallback() : CollisionCallback(nullptr) {}
    CollisionCallback(std::shared_ptr<Registry> registry) : registry(registry) {}

    void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
    void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {}
	void onWake(PxActor** actors, PxU32 count) override {}
	void onSleep(PxActor** actors, PxU32 count) override {}
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override {}
	static PxFilterFlags FilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);
private:
    std::shared_ptr<Registry> registry;
};