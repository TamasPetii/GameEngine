#include "RenderSystem.h"

namespace Ecs
{
	bool RenderSystem::IsRenderable(Entity* entity)
	{
		if (ComponentManager::HasComponent<RenderComponent, MeshComponent>(entity))
		{
			auto renderComponent = ComponentManager::GetComponent<RenderComponent>(entity);
			auto meshComponent = ComponentManager::GetComponent<MeshComponent>(entity);

			return renderComponent->Get_IsVisible() && meshComponent->Get_MeshSource() != nullptr;
		}
		
		return false;
	}
}