#include "TransformSystem.h"

namespace Ecs
{
	glm::mat4 TransformSystem::TransformMatrixFull(Entity* entity)
	{
		if (entity != nullptr && entity->HasComponent<TransformComponent>())
		{
			auto transform = entity->GetComponent<TransformComponent>();

			return TransformMatrixFull(entity->Get_Parent()) * TransformMatrixModel(entity);
		}

		return glm::mat4(1);
	}

	glm::mat4 TransformSystem::TransformMatrixModel(Entity* entity)
	{
		if (entity != nullptr && entity->HasComponent<TransformComponent>())
		{
			auto transform = entity->GetComponent<TransformComponent>();

			return glm::translate(transform->Get_Translation())
				 * glm::rotate(glm::radians(transform->Get_Rotation().x), glm::vec3(1, 0, 0))
				 * glm::rotate(glm::radians(transform->Get_Rotation().y), glm::vec3(0, 1, 0))
				 * glm::rotate(glm::radians(transform->Get_Rotation().z), glm::vec3(0, 0, 1))
				 * glm::scale(transform->Get_Scale());
		}

		return glm::mat4(1);
	}

	glm::mat4 TransformSystem::TransformMatrixParent(Entity* entity)
	{
		if (entity != nullptr && entity->Get_Parent() != nullptr && entity->HasComponent<TransformComponent>())
		{
			return TransformMatrixModel(entity->Get_Parent());
		}

		return glm::mat4(1);
	}
}
