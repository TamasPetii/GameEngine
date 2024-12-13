#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>
#include <memory>

using namespace physx;

struct ENGINE_API ConvexColliderComponent
{
	ConvexColliderComponent();

	bool isTrigger;
	PxConvexMeshGeometry convexMeshGeometry;
	std::shared_ptr<PxConvexMesh> convexMesh;
};