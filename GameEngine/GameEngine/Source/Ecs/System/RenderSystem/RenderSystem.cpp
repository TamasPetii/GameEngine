#include "RenderSystem.h"

namespace Ecs
{
	bool RenderSystem::IsRenderable(Entity* entity)
	{
		if (entity != nullptr && entity->HasComponent<RenderComponent, MeshComponent>())
		{
			auto renderComponent = entity->GetComponent<RenderComponent>();
			auto meshComponent = entity->GetComponent<MeshComponent>();

			return renderComponent->Get_IsVisible() && meshComponent->Get_MeshSource() != nullptr;
		}
		
		return false;
	}
}