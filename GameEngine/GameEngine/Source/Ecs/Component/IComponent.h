#pragma once
#include "../../ISerializable.h"
#include "../../IClonable.h"

namespace Ecs
{
	class IComponent : public ISerializable, public ICloneable
	{
	public:
		virtual ~IComponent() = default;
	};
}