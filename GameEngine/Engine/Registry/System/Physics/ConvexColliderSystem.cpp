#include "ConvexColliderSystem.h"
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>

#include "Logger/Logger.h"
#include "Model/Model.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/TransformComponent.h"
#include "Registry/Component/Object/ModelComponent.h"
#include "Registry/Component/Object/ShapeComponent.h"
#include "Registry/Component/Physics/ConvexColliderComponent.h"
#include "Render/Geometry/Geometry.h"

void ConvexColliderSystem::OnStart(std::shared_ptr<Registry> registry, PxPhysics* physics)
{

}

void ConvexColliderSystem::OnUpdate(std::shared_ptr<Registry> registry, PxPhysics* physics)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto convexColliderPool = registry->GetComponentPool<ConvexColliderComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	if (!transformPool || !convexColliderPool)
		return;

	std::for_each(std::execution::seq, convexColliderPool->GetDenseEntitiesArray().begin(), convexColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			bool hasShape = shapePool && shapePool->HasComponent(entity) && shapePool->GetComponent(entity).shape;
			bool hasModel = modelPool && modelPool->HasComponent(entity) && modelPool->GetComponent(entity).model;
			if (transformPool->HasComponent(entity) && (convexColliderPool->IsFlagSet(entity, UPDATE_FLAG) || transformPool->IsFlagSet(entity, CUSTOM1_FLAG)) && (hasShape || hasModel))
			{
				auto& convexCollider = convexColliderPool->GetComponent(entity);

				PxConvexMeshDesc convexDesc;
				convexDesc.points.stride = sizeof(PxVec3);
				convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

				if (hasModel)
				{
					auto& modelComponent = modelPool->GetComponent(entity);
					//This contains duplicate vertex positions!!!
					convexDesc.points.count = modelComponent.model->m_VertexPositions.size();
					convexDesc.points.data = modelComponent.model->m_VertexPositions.data();
				}
				else if (hasShape)
				{
					auto& shapeComponent = shapePool->GetComponent(entity);
					convexDesc.points.count = shapeComponent.shape->GetSurfacePoints().size();
					convexDesc.points.data = shapeComponent.shape->GetSurfacePoints().data();
				}
				else
				{
					//This should be never
				}

				PxTolerancesScale scale;
				PxCookingParams params(scale);

				PxDefaultMemoryOutputStream buf;
				PxConvexMeshCookingResult::Enum result;
				if (!PxCookConvexMesh(params, convexDesc, buf, &result))
					LOG_ERROR("MeshColliderSystem", "Error occured while cooking triangle mesh!");

				PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
				PxConvexMesh* convexMesh = physics->createConvexMesh(input);

				//What about parent transform?
				auto& transformComponent = transformPool->GetComponent(entity);
				PxVec3 size = PxVec3(transformComponent.scale.x, transformComponent.scale.y, transformComponent.scale.z);
				convexCollider.convexMeshGeometry = PxConvexMeshGeometry(convexMesh, PxMeshScale(size));
				
				convexCollider.convexMesh = std::shared_ptr<PxConvexMesh>(convexMesh, [](PxConvexMesh* ptr) {
					if (ptr)
					{
						//std::cout << "[ConvexCollider] - Released PxConvexMesh" << std::endl;
						ptr->release();
					}
				});

				convexColliderPool->ResFlag(entity, UPDATE_FLAG);
				convexColliderPool->SetFlag(entity, CHANGED_FLAG);
			}
		}
	);
}

void ConvexColliderSystem::OnEnd(std::shared_ptr<Registry> registry)
{
	auto convexColliderPool = registry->GetComponentPool<ConvexColliderComponent>();

	if (!convexColliderPool)
		return;

	std::for_each(std::execution::seq, convexColliderPool->GetDenseEntitiesArray().begin(), convexColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			convexColliderPool->ResFlag(entity, CHANGED_FLAG);
		}
	);
}


nlohmann::json ConvexColliderSystem::Serialize(Registry* registry, Entity entity)
{
	auto& convexColliderComponent = registry->GetComponent<ConvexColliderComponent>(entity);

	nlohmann::json data;
	data["isTrigger"] = convexColliderComponent.isTrigger;
	return data;
}

void ConvexColliderSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& convexColliderComponent = registry->GetComponent<ConvexColliderComponent>(entity);

	if (data.find("isTrigger") != data.end())
		convexColliderComponent.isTrigger = data["isTrigger"];

	registry->SetFlag<ConvexColliderComponent>(entity, UPDATE_FLAG);
}