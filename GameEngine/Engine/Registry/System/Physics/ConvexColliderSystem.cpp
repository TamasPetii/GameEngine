#include "ConvexColliderSystem.h"

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

	if (!transformPool || !convexColliderPool )
		return;

	std::for_each(std::execution::seq, convexColliderPool->GetDenseEntitiesArray().begin(), convexColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			bool hasShape = shapePool && shapePool->HasComponent(entity) && shapePool->GetComponent(entity).shape;
			bool hasModel = modelPool && modelPool->HasComponent(entity) && modelPool->GetComponent(entity).model;
			if (convexColliderPool->IsFlagSet(entity, UPDATE_FLAG) && transformPool->HasComponent(entity) && (hasShape || hasModel))
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
					std::cout << "Convex Error" << std::endl;

				PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
				convexCollider.convexMesh = physics->createConvexMesh(input);

				//What about parent transform?
				auto& transformComponent = transformPool->GetComponent(entity);
				PxVec3 size = PxVec3(transformComponent.scale.x, transformComponent.scale.y, transformComponent.scale.z);
				convexCollider.convexMeshGeometry = PxConvexMeshGeometry(convexCollider.convexMesh, PxMeshScale(size));
			
				convexColliderPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);
}

nlohmann::json ConvexColliderSystem::Serialize(Registry* registry, Entity entity)
{
	//auto& meshColliderComponent = registry->GetComponent<MeshColliderComponent>(entity);

	nlohmann::json data;
	return data;
}

void ConvexColliderSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	/*
	auto& meshCollider = registry->GetComponent<MeshCollider>(entity);

	registry->SetFlag<MeshCollider>(entity, REGENERATE_FLAG);
	registry->SetFlag<MeshCollider>(entity, UPDATE_FLAG);
	*/
}