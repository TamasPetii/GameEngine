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

					PxMaterial* material = physics->createMaterial(rigidbodyStaticComponent.sFriction, rigidbodyStaticComponent.dFriction, rigidbodyStaticComponent.restitution);
					PxShape* shape = physics->createShape(*colliderGeometry, *material);
					shape->setContactOffset(0.01f);
					PxRigidStatic* staticActor = physics->createRigidStatic(PxTransform(transfromComponent.translate.x, transfromComponent.translate.y, transfromComponent.translate.z));
					staticActor->attachShape(*shape);
					scene->addActor(*staticActor);

					rigidbodyStaticComponent.material = std::shared_ptr<PxMaterial>(material, [](PxMaterial* ptr) {
						if (ptr)
						{
							//std::cout << "[StaticRigidbody] - Released PxMaterial" << std::endl;
							ptr->release();
						}
						});

					rigidbodyStaticComponent.shape = std::shared_ptr<PxShape>(shape, [](PxShape* ptr) {
						if (ptr)
						{
							//std::cout << "[StaticRigidbody] - Released PxShape" << std::endl;
							ptr->release();
						}
						});

					rigidbodyStaticComponent.staticActor = std::shared_ptr<PxRigidStatic>(staticActor, [](PxRigidStatic* ptr) {
						if (ptr)
						{
							//std::cout << "[StaticRigidbody] - Released PxRigidStatic" << std::endl;
							ptr->release();
						}
						});
				}

				staticRigidbodyPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);
}

void RigidbodyStaticSystem::UpdateRigidbodyGlobalPose(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto staticRigidbodyPool = registry->GetComponentPool<RigidbodyStaticComponent>();

	if (!staticRigidbodyPool || !transformPool)
		return;

	std::for_each(std::execution::seq, staticRigidbodyPool->GetDenseEntitiesArray().begin(), staticRigidbodyPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->HasComponent(entity) && staticRigidbodyPool->GetComponent(entity).staticActor)
			{
				auto& staticRigidbodyComponent = staticRigidbodyPool->GetComponent(entity);
				auto& transfromComponent = transformPool->GetComponent(entity);

				PxTransform pxTransform = staticRigidbodyComponent.staticActor->getGlobalPose();
				glm::quat rotation = glm::quat(glm::radians(transfromComponent.rotate));
				PxVec3 pxPosition = PxVec3(transfromComponent.translate.x, transfromComponent.translate.y, transfromComponent.translate.z);
				PxQuat pxRotation = PxQuat(rotation.x, rotation.y, rotation.z, rotation.w);
				staticRigidbodyComponent.staticActor->setGlobalPose(PxTransform(pxPosition, pxRotation));
			}
		});
}

nlohmann::json RigidbodyStaticSystem::Serialize(Registry* registry, Entity entity)
{
	auto& rigidbodyStaticComponent = registry->GetComponent<RigidbodyStaticComponent>(entity);

	nlohmann::json data;
	data["sFriction"] = rigidbodyStaticComponent.sFriction;
	data["dFriction"] = rigidbodyStaticComponent.dFriction;
	data["restitution"] = rigidbodyStaticComponent.restitution;
	return data;
}

void RigidbodyStaticSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& rigidbodyStaticComponent = registry->GetComponent<RigidbodyStaticComponent>(entity);
	rigidbodyStaticComponent.sFriction = data["sFriction"];
	rigidbodyStaticComponent.dFriction = data["dFriction"];
	rigidbodyStaticComponent.restitution = data["restitution"];
	registry->SetFlag<RigidbodyStaticComponent>(entity, UPDATE_FLAG);
}