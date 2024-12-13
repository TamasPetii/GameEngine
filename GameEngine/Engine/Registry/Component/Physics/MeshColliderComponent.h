#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>
#include <memory>

using namespace physx;

struct ENGINE_API MeshColliderComponent
{
	MeshColliderComponent();

	bool isTrigger;
	PxTriangleMeshGeometry triangleMeshGeometry;
	std::shared_ptr<PxTriangleMesh> triangleMesh;
};