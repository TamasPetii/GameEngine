#pragma once
#include "EngineApi.h"
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxSimulationEventCallback.h>

#include "Registry/Registry.h"
#include "Registry/Component/ScriptComponent.h"

using namespace physx;

class ENGINE_API CollisionCallback : public PxSimulationEventCallback {
public:
    CollisionCallback() : CollisionCallback(nullptr) {}
    CollisionCallback(std::shared_ptr<Registry> registry) : registry(registry) {}

    void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
    void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override { std::cout << "onConstraintBreak is Triggered" << std::endl; }
	void onWake(PxActor** actors, PxU32 count) override { std::cout << "onWake is Triggered" << std::endl; }
	void onSleep(PxActor** actors, PxU32 count) override { std::cout << "onSleep is Triggered" << std::endl; }
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override { std::cout << "onAdvance is Triggered" << std::endl; }
	static PxFilterFlags FilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);
private:
    std::shared_ptr<Registry> registry;
};