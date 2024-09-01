#pragma once
#include "EngineApi.h"
#include <physx/PxPhysicsAPI.h>

#include <array>
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

using namespace physx;

struct ENGINE_API ConvexColliderComponent : public Component
{
	PxConvexMesh* convexMesh = nullptr;
	PxConvexMeshGeometry convexMeshGeometry;
};