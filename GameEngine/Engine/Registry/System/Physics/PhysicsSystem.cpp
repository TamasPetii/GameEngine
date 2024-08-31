#include "PhysicsSystem.h"

void PhysicsSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void PhysicsSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
}

nlohmann::json PhysicsSystem::Serialize(Registry* registry, Entity entity)
{
	nlohmann::json data;
	return data;
}

void PhysicsSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
}