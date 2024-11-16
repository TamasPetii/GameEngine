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
			bool hasBoxCollider = boxColliderPool && boxColliderPool->HasComponent(entity);
			bool hasSphereCollider = sphereColliderPool && sphereColliderPool->HasComponent(entity);
			bool hasConvexCollider = convexColliderPool && convexColliderPool->HasComponent(entity) && convexColliderPool->GetComponent(entity).convexMesh;
			bool hasMeshCollider = meshColliderPool && meshColliderPool->HasComponent(entity) && meshColliderPool->GetComponent(entity).triangleMesh;
			bool boxColliderChanged = hasBoxCollider && boxColliderPool->IsFlagSet(entity, CHANGED_FLAG);
			bool sphereColliderChanged = hasSphereCollider && sphereColliderPool->IsFlagSet(entity, CHANGED_FLAG);
			bool meshColliderChanged = hasMeshCollider && meshColliderPool->IsFlagSet(entity, CHANGED_FLAG);
			bool convexColliderChanged = hasConvexCollider && convexColliderPool->IsFlagSet(entity, CHANGED_FLAG);

			if (transformPool->HasComponent(entity) && (dynamicRigidbodyPool->IsFlagSet(entity, UPDATE_FLAG) || boxColliderChanged || sphereColliderChanged || meshColliderChanged || convexColliderChanged))
			{
				auto& rigidbodyDynamicComponent = dynamicRigidbodyPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);

				if (hasBoxCollider || hasSphereCollider || hasConvexCollider || hasMeshCollider)
				{
					if (scene && rigidbodyDynamicComponent.dynamicActor)
						scene->removeActor(*rigidbodyDynamicComponent.dynamicActor);

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

					PxMaterial* material = physics->createMaterial(rigidbodyDynamicComponent.sFriction, rigidbodyDynamicComponent.dFriction, rigidbodyDynamicComponent.restitution);
					PxShape* shape = physics->createShape(*colliderGeometry, *material);
					shape->setSimulationFilterData(filterData);
					shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
					shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
					PxRigidDynamic* dynamicActor = physics->createRigidDynamic(PxTransform(transformedOrigin.x, transformedOrigin.y, transformedOrigin.z));
					dynamicActor->setMass(rigidbodyDynamicComponent.mass);
					dynamicActor->attachShape(*shape);

					dynamicActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, rigidbodyDynamicComponent.disableGravity);
					dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, rigidbodyDynamicComponent.isKinematic);
					dynamicActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidbodyDynamicComponent.lockRotation[0]);
					dynamicActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, rigidbodyDynamicComponent.lockRotation[1]);
					dynamicActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, rigidbodyDynamicComponent.lockRotation[2]);
					dynamicActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, rigidbodyDynamicComponent.lockPosition[0]);
					dynamicActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, rigidbodyDynamicComponent.lockPosition[1]);
					dynamicActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, rigidbodyDynamicComponent.lockPosition[2]);

					dynamicActor->userData = new Entity(entity);
					scene->addActor(*dynamicActor);

					rigidbodyDynamicComponent.material = std::shared_ptr<PxMaterial>(material, [](PxMaterial* ptr) {
						if (ptr)
						{
							//std::cout << "[DynamicRigidbody] - Released PxMaterial" << std::endl;
							ptr->release();
						}
					});

					rigidbodyDynamicComponent.shape = std::shared_ptr<PxShape>(shape, [](PxShape* ptr) {
						if (ptr)
						{
							//std::cout << "[DynamicRigidbody] - Released PxShape" << std::endl;
							ptr->release();
						}
					});

					rigidbodyDynamicComponent.dynamicActor = std::shared_ptr<PxRigidDynamic>(dynamicActor, [](PxRigidDynamic* ptr) {
						if (ptr)
						{
							if (ptr->userData)
								delete ptr->userData;
							ptr->release();
							//std::cout << "[DynamicRigidbody] - Released PxRigidDynamic" << std::endl;
						}
					});

					dynamicRigidbodyPool->ResFlag(entity, UPDATE_FLAG);
				}

			}
		}
	);
}

void RigidbodyDynamicSystem::UpdateRigidbodyGlobalPose(std::shared_ptr<Registry> registry, bool init)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto dynamicRigidbodyPool = registry->GetComponentPool<RigidbodyDynamicComponent>();
	auto boxColliderPool = registry->GetComponentPool<BoxColliderComponent>();
	auto sphereColliderPool = registry->GetComponentPool<SphereColliderComponent>();
	auto convexColliderPool = registry->GetComponentPool<ConvexColliderComponent>();
	auto meshColliderPool = registry->GetComponentPool<MeshColliderComponent>();

	if (!dynamicRigidbodyPool || !transformPool)
		return;

	std::for_each(std::execution::seq, dynamicRigidbodyPool->GetDenseEntitiesArray().begin(), dynamicRigidbodyPool->GetDenseEntitiesArray().end(),
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

			if (transformPool->HasComponent(entity) && dynamicRigidbodyPool->GetComponent(entity).dynamicActor && 
				(hasBoxCollider || hasSphereCollider || hasConvexCollider || hasMeshCollider) &&
				(transformPool->IsFlagSet(entity, CHANGED_FLAG) || init))
			{
				auto& dynamicRigidbodyComponent = dynamicRigidbodyPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);

				glm::vec3 transformedOrigin = hasBoxCollider ? boxColliderPool->GetComponent(entity).transformedOrigin
											: hasSphereCollider ? sphereColliderPool->GetComponent(entity).transformedOrigin
											: transformComponent.translate;

				PxTransform pxTransform = dynamicRigidbodyComponent.dynamicActor->getGlobalPose();
				glm::quat rotation = glm::quat(glm::radians(transformComponent.rotate));
				PxVec3 pxPosition = PxVec3(transformedOrigin.x, transformedOrigin.y, transformedOrigin.z);
				PxQuat pxRotation = PxQuat(rotation.x, rotation.y, rotation.z, rotation.w);

				dynamicRigidbodyComponent.dynamicActor->setGlobalPose(PxTransform(pxPosition, pxRotation));
			}
		});
}

void RigidbodyDynamicSystem::FetchRigidbodyGlobalPose(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto dynamicRigidbodyPool = registry->GetComponentPool<RigidbodyDynamicComponent>();
	auto boxColliderPool = registry->GetComponentPool<BoxColliderComponent>();
	auto sphereColliderPool = registry->GetComponentPool<SphereColliderComponent>();
	auto convexColliderPool = registry->GetComponentPool<ConvexColliderComponent>();
	auto meshColliderPool = registry->GetComponentPool<MeshColliderComponent>();

	if (!dynamicRigidbodyPool || !transformPool)
		return;

	std::for_each(std::execution::par, dynamicRigidbodyPool->GetDenseEntitiesArray().begin(), dynamicRigidbodyPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			bool hasBoxCollider = boxColliderPool && boxColliderPool->HasComponent(entity);
			bool hasSphereCollider = sphereColliderPool && sphereColliderPool->HasComponent(entity);
			if (transformPool->HasComponent(entity) && dynamicRigidbodyPool->GetComponent(entity).dynamicActor)
			{
				auto& dynamicRigidbodyComponent = dynamicRigidbodyPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				
				glm::vec3 transformedOrigin = hasBoxCollider ? boxColliderPool->GetComponent(entity).transformedOrigin
					: hasSphereCollider ? sphereColliderPool->GetComponent(entity).transformedOrigin
					: transformComponent.translate;

				PxTransform pxTransform = dynamicRigidbodyComponent.dynamicActor->getGlobalPose();
				transformComponent.translate += glm::vec3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z) - transformedOrigin;

				// Extract rotation (convert from PxQuat to glm::quat)
				PxQuat pxQuat = pxTransform.q;
				glm::quat glmQuat(pxQuat.w, pxQuat.x, pxQuat.y, pxQuat.z);

				// Convert to Euler angles (in radians)
				glm::vec3 eulerAngles = glm::eulerAngles(glmQuat);
				transformComponent.rotate += glm::degrees(eulerAngles) - transformComponent.rotate;  // Convert to degrees if needed
			
				transformPool->SetFlag(entity, UPDATE_FLAG);
			}
		}
	);
}

nlohmann::json RigidbodyDynamicSystem::Serialize(Registry* registry, Entity entity)
{
	auto& rigidbodyDynamicComponent = registry->GetComponent<RigidbodyDynamicComponent>(entity);

	nlohmann::json data;
	data["mass"] = rigidbodyDynamicComponent.mass;
	data["sFriction"] = rigidbodyDynamicComponent.sFriction;
	data["dFriction"] = rigidbodyDynamicComponent.dFriction;
	data["restitution"] = rigidbodyDynamicComponent.restitution;
	data["disableGravity"] = rigidbodyDynamicComponent.disableGravity;
	data["isKinematic"] = rigidbodyDynamicComponent.isKinematic;
	data["lockRotation"]["x"] = rigidbodyDynamicComponent.lockRotation[0];
	data["lockRotation"]["y"] = rigidbodyDynamicComponent.lockRotation[1];
	data["lockRotation"]["z"] = rigidbodyDynamicComponent.lockRotation[2];
	data["lockPosition"]["x"] = rigidbodyDynamicComponent.lockPosition[0];
	data["lockPosition"]["y"] = rigidbodyDynamicComponent.lockPosition[1];
	data["lockPosition"]["z"] = rigidbodyDynamicComponent.lockPosition[2];
	return data;
}

void RigidbodyDynamicSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& rigidbodyDynamicComponent = registry->GetComponent<RigidbodyDynamicComponent>(entity);
	rigidbodyDynamicComponent.mass = data["mass"];
	rigidbodyDynamicComponent.sFriction = data["sFriction"];
	rigidbodyDynamicComponent.dFriction = data["dFriction"];
	rigidbodyDynamicComponent.restitution = data["restitution"];

	rigidbodyDynamicComponent.disableGravity = data["disableGravity"];
	rigidbodyDynamicComponent.isKinematic = data["isKinematic"];
	rigidbodyDynamicComponent.lockRotation[0] = data["lockRotation"]["x"];
	rigidbodyDynamicComponent.lockRotation[1] = data["lockRotation"]["y"];
	rigidbodyDynamicComponent.lockRotation[2] = data["lockRotation"]["z"];

	if (data.find("lockPosition") != data.end())
	{
		rigidbodyDynamicComponent.lockPosition[0] = data["lockPosition"]["x"];
		rigidbodyDynamicComponent.lockPosition[1] = data["lockPosition"]["y"];
		rigidbodyDynamicComponent.lockPosition[2] = data["lockPosition"]["z"];
	}

	registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
}