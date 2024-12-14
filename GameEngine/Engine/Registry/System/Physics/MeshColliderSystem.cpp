#include "MeshColliderSystem.h"

#include <algorithm>
#include <execution>
#include <glm/glm.hpp>

#include "Logger/Logger.h"
#include "Model/Model.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/TransformComponent.h"
#include "Registry/Component/Object/ShapeComponent.h"
#include "Registry/Component/Object/ModelComponent.h"
#include "Registry/Component/Physics/MeshColliderComponent.h"
#include "Render/Geometry/Geometry.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"

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
			if (transformPool->HasComponent(entity) && (meshColliderPool->IsFlagSet(entity, UPDATE_FLAG) || transformPool->IsFlagSet(entity, CUSTOM1_FLAG)) && (hasShape || hasModel))
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
					LOG_ERROR("MeshColliderSystem", "Error occured while cooking triangle mesh!");
				}

				PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
				PxTriangleMesh* triangleMesh = physics->createTriangleMesh(readBuffer);

				auto& transformComponent = transformPool->GetComponent(entity);
				PxVec3 size = PxVec3(transformComponent.scale.x, transformComponent.scale.y, transformComponent.scale.z);
				meshCollider.triangleMeshGeometry = PxTriangleMeshGeometry(triangleMesh, PxMeshScale(size));

				meshCollider.triangleMesh = std::shared_ptr<PxTriangleMesh>(triangleMesh, [](PxTriangleMesh* ptr) {
					if (ptr)
					{
						//std::cout << "[MeshCollider] - Released PxTriangleMesh" << std::endl;
						ptr->release();
					}
				});

				meshColliderPool->ResFlag(entity, UPDATE_FLAG);
				meshColliderPool->SetFlag(entity, CHANGED_FLAG);
			}
		}
	);
}

void MeshColliderSystem::OnEnd(std::shared_ptr<Registry> registry)
{
	auto meshColliderPool = registry->GetComponentPool<MeshColliderComponent>();

	if (!meshColliderPool)
		return;

	std::for_each(std::execution::seq, meshColliderPool->GetDenseEntitiesArray().begin(), meshColliderPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			meshColliderPool->ResFlag(entity, CHANGED_FLAG);
		}
	);
}

nlohmann::json MeshColliderSystem::Serialize(Registry* registry, Entity entity)
{
	auto& meshColliderComponent = registry->GetComponent<MeshColliderComponent>(entity);
	nlohmann::json data;
	data["isTrigger"] = meshColliderComponent.isTrigger;
	return data;
}

void MeshColliderSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& meshColliderComponent = registry->GetComponent<MeshColliderComponent>(entity);

	if (data.find("isTrigger") != data.end())
		meshColliderComponent.isTrigger = data["isTrigger"];

	registry->SetFlag<MeshColliderComponent>(entity, UPDATE_FLAG);
}