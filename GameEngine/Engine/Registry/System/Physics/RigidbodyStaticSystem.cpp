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
			bool hasBoxCollider = boxColliderPool && boxColliderPool->HasComponent(entity);
			bool hasSphereCollider = sphereColliderPool && sphereColliderPool->HasComponent(entity);
			bool hasConvexCollider = convexColliderPool && convexColliderPool->HasComponent(entity);
			bool hasMeshCollider = meshColliderPool && meshColliderPool->HasComponent(entity);
			bool boxColliderChanged = hasBoxCollider && boxColliderPool->IsFlagSet(entity, CHANGED_FLAG);
			bool sphereColliderChanged = hasSphereCollider && sphereColliderPool->IsFlagSet(entity, CHANGED_FLAG);
			bool meshColliderChanged = hasMeshCollider && meshColliderPool->IsFlagSet(entity, CHANGED_FLAG);
			bool convexColliderChanged = hasConvexCollider && convexColliderPool->IsFlagSet(entity, CHANGED_FLAG);

			if (transformPool->HasComponent(entity) && (staticRigidbodyPool->IsFlagSet(entity, UPDATE_FLAG) || boxColliderChanged || sphereColliderChanged || meshColliderChanged || convexColliderChanged))
			{
				auto& rigidbodyStaticComponent = staticRigidbodyPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);

				if (hasBoxCollider || hasSphereCollider || hasConvexCollider || hasMeshCollider)
				{
					if (scene && rigidbodyStaticComponent.staticActor)
						scene->removeActor(*rigidbodyStaticComponent.staticActor);

					PxGeometry* colliderGeometry = hasBoxCollider ? static_cast<PxGeometry*>(&boxColliderPool->GetComponent(entity).boxGeometry)
						: hasSphereCollider ? static_cast<PxGeometry*>(&sphereColliderPool->GetComponent(entity).sphereGeometry)
						: hasConvexCollider ? static_cast<PxGeometry*>(&convexColliderPool->GetComponent(entity).convexMeshGeometry)
						: static_cast<PxGeometry*>(&meshColliderPool->GetComponent(entity).triangleMeshGeometry);


					glm::vec3 transformedOrigin = hasBoxCollider ? boxColliderPool->GetComponent(entity).transformedOrigin
						: hasSphereCollider ? sphereColliderPool->GetComponent(entity).transformedOrigin
						: transformComponent.translate;

					bool isTrigger = hasBoxCollider ? boxColliderPool->GetComponent(entity).isTrigger
						: hasSphereCollider ? sphereColliderPool->GetComponent(entity).isTrigger
						: hasConvexCollider ? convexColliderPool->GetComponent(entity).isTrigger
						: meshColliderPool->GetComponent(entity).isTrigger;

					PxFilterData filterData;
					filterData.word0 = 0x1;
					filterData.word1 = 0x1;

					PxMaterial* material = physics->createMaterial(rigidbodyStaticComponent.sFriction, rigidbodyStaticComponent.dFriction, rigidbodyStaticComponent.restitution);
					PxShape* shape = physics->createShape(*colliderGeometry, *material);
					shape->setSimulationFilterData(filterData);
					shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
					shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
					PxRigidStatic* staticActor = physics->createRigidStatic(PxTransform(transformedOrigin.x, transformedOrigin.y, transformedOrigin.z));
					staticActor->attachShape(*shape);

					staticActor->userData = new Entity(entity);
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
							ptr->release();
							//std::cout << "[StaticRigidbody] - Released PxShape" << std::endl;
						}
						});

					rigidbodyStaticComponent.staticActor = std::shared_ptr<PxRigidStatic>(staticActor, [](PxRigidStatic* ptr) {
						if (ptr)
						{
							if (ptr->userData)
								delete ptr->userData;
							ptr->release();
							//std::cout << "[StaticRigidbody] - Released PxRigidStatic" << std::endl;
						}
						});
				}

				staticRigidbodyPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);
}

void RigidbodyStaticSystem::UpdateRigidbodyGlobalPose(std::shared_ptr<Registry> registry, bool init)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto staticRigidbodyPool = registry->GetComponentPool<RigidbodyStaticComponent>();
	auto boxColliderPool = registry->GetComponentPool<BoxColliderComponent>();
	auto sphereColliderPool = registry->GetComponentPool<SphereColliderComponent>();
	auto convexColliderPool = registry->GetComponentPool<ConvexColliderComponent>();
	auto meshColliderPool = registry->GetComponentPool<MeshColliderComponent>();

	if (!staticRigidbodyPool || !transformPool)
		return;

	std::for_each(std::execution::seq, staticRigidbodyPool->GetDenseEntitiesArray().begin(), staticRigidbodyPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			bool hasBoxCollider = boxColliderPool && boxColliderPool->HasComponent(entity);
			bool hasSphereCollider = sphereColliderPool && sphereColliderPool->HasComponent(entity);
			bool hasConvexCollider = convexColliderPool && convexColliderPool->HasComponent(entity);
			bool hasMeshCollider = meshColliderPool && meshColliderPool->HasComponent(entity);

			/*
			bool boxColliderChanged = hasBoxCollider && boxColliderPool->IsFlagSet(entity, CHANGED_FLAG);
			bool sphereColliderChanged = hasSphereCollider && sphereColliderPool->IsFlagSet(entity, CHANGED_FLAG);
			bool convexColliderChanged = hasConvexCollider && convexColliderPool->IsFlagSet(entity, CHANGED_FLAG);
			bool meshColliderChanged = meshColliderPool && meshColliderPool->IsFlagSet(entity, CHANGED_FLAG);
			*/

			if (transformPool->HasComponent(entity) && staticRigidbodyPool->GetComponent(entity).staticActor &&
				(hasBoxCollider || hasSphereCollider || hasConvexCollider || hasMeshCollider) &&
				(transformPool->IsFlagSet(entity, CHANGED_FLAG) || init))
			{
				auto& staticRigidbodyComponent = staticRigidbodyPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);

				glm::vec3 transformedOrigin = hasBoxCollider ? boxColliderPool->GetComponent(entity).transformedOrigin
					: hasSphereCollider ? sphereColliderPool->GetComponent(entity).transformedOrigin
					: transformComponent.translate;

				PxTransform pxTransform = staticRigidbodyComponent.staticActor->getGlobalPose();
				glm::quat rotation = glm::quat(glm::radians(transformComponent.rotate));
				PxVec3 pxPosition = PxVec3(transformedOrigin.x, transformedOrigin.y, transformedOrigin.z);
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