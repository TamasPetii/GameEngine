#include "RigidbodyStaticSystem.h"

void RigidbodyStaticSystem::OnStart(std::shared_ptr<Registry> registry, PxPhysics* physics, PxScene* scene)
{

}

void RigidbodyStaticSystem::OnUpdate(std::shared_ptr<Registry> registry, PxPhysics* physics, PxScene* scene)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto staticRigidbodyPool = registry->GetComponentPool<RigidbodyStaticComponent>();
	auto boxColliderPool = registry->GetComponentPool<BoxColliderComponent>();
	auto sphereColliderPool = registry->GetComponentPool<SphereColliderComponent>();
	auto convexColliderPool = registry->GetComponentPool<ConvexColliderComponent>();
	auto meshColliderPool = registry->GetComponentPool<MeshColliderComponent>();

	if (!transformPool || !staticRigidbodyPool)
		return;

	std::for_each(std::execution::seq, staticRigidbodyPool->GetDenseEntitiesArray().begin(), staticRigidbodyPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (staticRigidbodyPool->IsFlagSet(entity, UPDATE_FLAG) && transformPool->HasComponent(entity))
			{
				auto& rigidbodyStaticComponent = staticRigidbodyPool->GetComponent(entity);
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

					rigidbodyStaticComponent.material = physics->createMaterial(rigidbodyStaticComponent.sFriction, rigidbodyStaticComponent.dFriction, rigidbodyStaticComponent.restitution);
					rigidbodyStaticComponent.shape = physics->createShape(*colliderGeometry, *rigidbodyStaticComponent.material);
					rigidbodyStaticComponent.shape->setContactOffset(0.01f);
					rigidbodyStaticComponent.staticActor = physics->createRigidStatic(PxTransform(transfromComponent.translate.x, transfromComponent.translate.y, transfromComponent.translate.z));
					rigidbodyStaticComponent.staticActor->attachShape(*rigidbodyStaticComponent.shape);
					scene->addActor(*rigidbodyStaticComponent.staticActor);
				}
			}
		}
	);
}

nlohmann::json RigidbodyStaticSystem::Serialize(Registry* registry, Entity entity)
{
	//auto& meshColliderComponent = registry->GetComponent<MeshColliderComponent>(entity);

	nlohmann::json data;
	return data;
}

void RigidbodyStaticSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	/*
	auto& meshCollider = registry->GetComponent<MeshCollider>(entity);

	registry->SetFlag<MeshCollider>(entity, REGENERATE_FLAG);
	registry->SetFlag<MeshCollider>(entity, UPDATE_FLAG);
	*/
}