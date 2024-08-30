#include "RigidbodyDynamicSystem.h"

void RigidbodyDynamicSystem::OnStart(std::shared_ptr<Registry> registry, PxPhysics* physics, PxScene* scene)
{

}

void RigidbodyDynamicSystem::OnUpdate(std::shared_ptr<Registry> registry, PxPhysics* physics, PxScene* scene)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto dynamicRigidbodyPool = registry->GetComponentPool<RigidbodyDynamicComponent>();
	auto boxColliderPool = registry->GetComponentPool<BoxColliderComponent>();
	auto sphereColliderPool = registry->GetComponentPool<SphereColliderComponent>();
	auto convexColliderPool = registry->GetComponentPool<ConvexColliderComponent>();
	auto meshColliderPool = registry->GetComponentPool<MeshColliderComponent>();

	if (!transformPool || !dynamicRigidbodyPool)
		return;

	std::for_each(std::execution::seq, dynamicRigidbodyPool->GetDenseEntitiesArray().begin(), dynamicRigidbodyPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (dynamicRigidbodyPool->IsFlagSet(entity, UPDATE_FLAG) && transformPool->HasComponent(entity))
			{
				auto& rigidbodyDynamicComponent = dynamicRigidbodyPool->GetComponent(entity);
				auto& transfromComponent = transformPool->GetComponent(entity);

				bool hasBoxCollider = boxColliderPool && boxColliderPool->HasComponent(entity);
				bool hasSphereCollider = sphereColliderPool && sphereColliderPool->HasComponent(entity);
				bool hasConvexCollider = convexColliderPool && convexColliderPool->HasComponent(entity);
				bool hasMeshCollider = meshColliderPool && meshColliderPool->HasComponent(entity);

				if (hasBoxCollider || hasSphereCollider || hasConvexCollider || hasMeshCollider)
				{
					PxGeometry* colliderGeometry = hasBoxCollider ? static_cast<PxGeometry*>(&boxColliderPool->GetComponent(entity).boxGeometry)
											     : hasSphereCollider ? static_cast<PxGeometry*>(&sphereColliderPool->GetComponent(entity).sphereGeometry)
												 : hasConvexCollider ? static_cast<PxGeometry*>(&convexColliderPool->GetComponent(entity).convexMeshGeometry)
												 : static_cast<PxGeometry*>(&meshColliderPool->GetComponent(entity).triangleMeshGeometry);

					rigidbodyDynamicComponent.material = physics->createMaterial(rigidbodyDynamicComponent.sFriction, rigidbodyDynamicComponent.dFriction, rigidbodyDynamicComponent.restitution);
					rigidbodyDynamicComponent.shape = physics->createShape(*colliderGeometry, *rigidbodyDynamicComponent.material);
					rigidbodyDynamicComponent.shape->setContactOffset(0.01f);
					rigidbodyDynamicComponent.dynamicActor = physics->createRigidDynamic(PxTransform(transfromComponent.translate.x, transfromComponent.translate.y, transfromComponent.translate.z));
					rigidbodyDynamicComponent.dynamicActor->setMass(rigidbodyDynamicComponent.mass);
					rigidbodyDynamicComponent.dynamicActor->attachShape(*rigidbodyDynamicComponent.shape);
					scene->addActor(*rigidbodyDynamicComponent.dynamicActor);
					//Scene out -> par
				}
			}
		}
	);
}

nlohmann::json RigidbodyDynamicSystem::Serialize(Registry* registry, Entity entity)
{
	//auto& meshColliderComponent = registry->GetComponent<MeshColliderComponent>(entity);

	nlohmann::json data;
	return data;
}

void RigidbodyDynamicSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	/*
	auto& meshCollider = registry->GetComponent<MeshCollider>(entity);

	registry->SetFlag<MeshCollider>(entity, REGENERATE_FLAG);
	registry->SetFlag<MeshCollider>(entity, UPDATE_FLAG);
	*/
}