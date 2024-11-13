#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <memory>
#include <vector>
#include <glm/glm.hpp>

using namespace physx;

struct ENGINE_API MeshColliderComponent
{
	bool isTrigger = false;
	PxTriangleMeshGeometry triangleMeshGeometry;
	std::shared_ptr<PxTriangleMesh> triangleMesh = nullptr;
};