#include "MeshColliderSystem.h"

void MeshColliderSystem::OnStart(std::shared_ptr<Registry> registry, PxPhysics* physics)
{

}

void MeshColliderSystem::OnUpdate(std::shared_ptr<Registry> registry, PxPhysics* physics)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto meshColliderPool = registry->GetComponentPool<MeshColliderComponent>();
	auto modelPool = registry->GetComponentPool<ModelComponent>();
	auto shapePool = registry->GetComponentPool<ShapeComponent>();

	if (!transformPool || !meshColliderPool)
		return;

	std::for_each(std::execution::seq, meshColliderPool->GetDenseEntitiesArray().begin(), meshColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			bool hasShape = shapePool && shapePool->HasComponent(entity) && shapePool->GetComponent(entity).shape;
			bool hasModel = modelPool && modelPool->HasComponent(entity) && modelPool->GetComponent(entity).model;
			if (meshColliderPool->IsFlagSet(entity, UPDATE_FLAG) && transformPool->HasComponent(entity) && (hasShape || hasModel))
			{
				auto& meshCollider = meshColliderPool->GetComponent(entity);

				PxTriangleMeshDesc meshDesc;
				meshDesc.points.stride = sizeof(PxVec3);
				meshDesc.triangles.stride = 3 * sizeof(PxU32);

				if (hasModel)
				{
					auto& modelComponent = modelPool->GetComponent(entity);
					meshDesc.points.count = modelComponent.model->GetVertexCount();
					meshDesc.points.data = modelComponent.model->m_VertexPositions.data();
					meshDesc.triangles.count = modelComponent.model->GetIndexCount() / 3;
					meshDesc.triangles.data = modelComponent.model->m_VertexIndices.data();
				}
				else if (hasShape)
				{
					auto& shapeComponent = shapePool->GetComponent(entity);
					meshDesc.points.count = shapeComponent.shape->GetVertexCount();
					meshDesc.points.data = shapeComponent.shape->GetVertices().data();
					meshDesc.triangles.count = shapeComponent.shape->GetIndexCount() / 3;
					meshDesc.triangles.data = shapeComponent.shape->GetIndices().data();
				}
				else
				{
					//This should be never
				}


				PxTolerancesScale scale;
				PxCookingParams params(scale);
				PxDefaultMemoryOutputStream writeBuffer;
				PxTriangleMeshCookingResult::Enum result;

				if (!PxCookTriangleMesh(params, meshDesc, writeBuffer, &result)) {
					std::cout << "Triangle Error" << std::endl;
				}

				PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
				meshCollider.triangleMesh = physics->createTriangleMesh(readBuffer);

				auto& transformComponent = transformPool->GetComponent(entity);
				PxVec3 size = PxVec3(transformComponent.scale.x, transformComponent.scale.y, transformComponent.scale.z);
				meshCollider.triangleMeshGeometry = PxTriangleMeshGeometry(meshCollider.triangleMesh, PxMeshScale(size));

				meshColliderPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);
}

nlohmann::json MeshColliderSystem::Serialize(Registry* registry, Entity entity)
{
	//auto& meshColliderComponent = registry->GetComponent<MeshColliderComponent>(entity);

	nlohmann::json data;
	return data;
}

void MeshColliderSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	/*
	auto& meshCollider = registry->GetComponent<MeshCollider>(entity);

	registry->SetFlag<MeshCollider>(entity, REGENERATE_FLAG);
	registry->SetFlag<MeshCollider>(entity, UPDATE_FLAG);
	*/
}