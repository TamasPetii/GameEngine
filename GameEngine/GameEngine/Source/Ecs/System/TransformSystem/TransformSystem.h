#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform2.hpp>

#include "../ISystem.h"

namespace Ecs
{
	class TransformSystem : public ISystem
	{
	public:
		static glm::mat4 TransformMatrixFull(Entity* entity);
		static glm::mat4 TransformMatrixModel(Entity* entity);
	};
}


