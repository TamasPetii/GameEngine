#include "MaterialSystem.h"

void MaterialSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void MaterialSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto mtDataSsbo = resourceManager->GetSsbo("MaterialData");
	auto mtDataSsboHandler = static_cast<MaterialGLSL*>(mtDataSsbo->MapBuffer(GL_WRITE_ONLY));
	auto materialPool = registry->GetComponentPool<MaterialComponent>();

	std::for_each(std::execution::par, materialPool->GetDenseEntitiesArray().begin(), materialPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (materialPool->IsFlagSet(entity, UPDATE_FLAG))
			{
				auto& materialComponent = materialPool->GetComponent(entity);
				auto index = materialPool->GetIndex(entity);

				mtDataSsboHandler[index] = MaterialGLSL(materialComponent);
				materialPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	mtDataSsbo->UnMapBuffer();
}
