#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <memory>
#include <array>
#include <glm/glm.hpp>

using namespace physx;

struct ENGINE_API ConvexColliderComponent
{
	bool isTrigger = false;
	PxConvexMeshGeometry convexMeshGeometry;
	std::shared_ptr<PxConvexMesh> convexMesh = nullptr;
};