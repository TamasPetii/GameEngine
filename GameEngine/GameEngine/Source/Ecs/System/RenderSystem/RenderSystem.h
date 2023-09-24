#pragma once
#include "../ISystem.h"

namespace Ecs
{
	class RenderSystem : public ISystem
	{
	public:
		static bool IsRenderable(Entity* entity);
	};
}

