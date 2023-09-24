#pragma once
#include "../Manager/ComponentManager/ComponentManager.h"
#include "../Manager/EntityManager/EntityManager.h"

namespace Ecs
{
	class ISystem
	{
	public:
		virtual ~ISystem() = default;
	};
}


