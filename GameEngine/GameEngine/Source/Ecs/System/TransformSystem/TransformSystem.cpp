#include "TransformSystem.h"

namespace Ecs
{
	glm::mat4 TransformSystem::TransformMatrixFull(Entity* entity)
	{
		if (entity != nullptr && ComponentManager::HasComponent<TransformComponent>(entity))
		{
			auto transform = ComponentManager::GetComponent<TransformComponent>(entity);

			return TransformSystem::TransformMatrixFull(EntityManager::Parent(entity))
				 * glm::translate(transform->Get_Translation())
				 * glm::rotate(glm::radians(transform->Get_Rotation().x), glm::vec3(1, 0, 0))
				 * glm::rotate(glm::radians(transform->Get_Rotation().y), glm::vec3(0, 1, 0))
				 * glm::rotate(glm::radians(transform->Get_Rotation().z), glm::vec3(0, 0, 1))
				 * glm::scale(transform->Get_Scale());
		}

		return glm::mat4(1);
	}

	glm::mat4 TransformSystem::TransformMatrixModel(Entity* entity)
	{
		if (entity != nullptr && ComponentManager::HasComponent<TransformComponent>(entity))
		{
			auto transform = ComponentManager::GetComponent<TransformComponent>(entity);

			return glm::translate(transform->Get_Translation())
				 * glm::rotate(glm::radians(transform->Get_Rotation().x), glm::vec3(1, 0, 0))
				 * glm::rotate(glm::radians(transform->Get_Rotation().y), glm::vec3(0, 1, 0))
				 * glm::rotate(glm::radians(transform->Get_Rotation().z), glm::vec3(0, 0, 1))
				 * glm::scale(transform->Get_Scale());
		}

		return glm::mat4(1);
	}
}
