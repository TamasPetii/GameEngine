#include "TransformSystem.h"

void TransformSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void TransformSystem::OnUpdate(std::shared_ptr<Registry> registry)
{	
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();

	static bool init = true;
	static TransformGLSL* tsDataSsboHandler = nullptr;
	if (init)
	{
		init = false; 
		auto tsDataSsbo = resourceManager->GetSsbo("TransformData");
		tsDataSsboHandler = static_cast<TransformGLSL*>(tsDataSsbo->MapBuffer(GL_WRITE_ONLY));
	}

	std::for_each(std::execution::par_unseq, transformPool->GetDenseEntitiesArray().begin(), transformPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (transformPool->IsFlagSet(entity, UPDATE_FLAG))
			{
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = transformPool->GetIndex(entity);

				transformComponent.modelTransform = glm::translate(transformComponent.translate)
					* glm::rotate(glm::radians(transformComponent.rotate.z), glm::vec3(0, 0, 1))
					* glm::rotate(glm::radians(transformComponent.rotate.y), glm::vec3(0, 1, 0))
					* glm::rotate(glm::radians(transformComponent.rotate.x), glm::vec3(1, 0, 0))
					* glm::scale(transformComponent.scale);

				transformComponent.modelTransformIT = glm::inverse(glm::transpose(transformComponent.modelTransform));

				transformComponent.fullTransform = transformComponent.modelTransform;
				transformComponent.fullTransformIT = transformComponent.modelTransformIT;

				tsDataSsboHandler[index] = TransformGLSL(transformComponent);
				transformPool->ResFlag(entity, UPDATE_FLAG);
				transformPool->SetFlag(entity, CHANGED_FLAG);
			}
		}
	);

	//tsDataSsbo->UnMapBuffer();
}

void TransformSystem::OnEnd(std::shared_ptr<Registry> registry)
{
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	std::for_each(std::execution::seq, transformPool->GetDenseEntitiesArray().begin(), transformPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			transformPool->ResFlag(entity, CHANGED_FLAG);
		}
	);
}