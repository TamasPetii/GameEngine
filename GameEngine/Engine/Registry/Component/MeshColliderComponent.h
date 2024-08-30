#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <vector>
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

using namespace physx;

struct ENGINE_API MeshColliderComponent : public Component
{
	PxTriangleMesh* triangleMesh = nullptr;
	PxTriangleMeshGeometry triangleMeshGeometry;
};