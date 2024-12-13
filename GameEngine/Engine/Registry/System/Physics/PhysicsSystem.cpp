#include "PhysicsSystem.h"
#include <algorithm>
#include <execution>

glm::vec3 PhysicsSystem::gravity = glm::vec3(0, -9.81, 0);

void PhysicsSystem::OnUpdate(physx::PxScene* gScene, float deltaTime)
{
	if (deltaTime < 0.1)
	{
		gScene->setGravity(PxVec3(gravity.x, gravity.y, gravity.z));
		gScene->simulate(deltaTime);
		gScene->fetchResults(true);
	}
}

nlohmann::json PhysicsSystem::Serialize()
{
	nlohmann::json data;
	data["gravity"]["x"] = PhysicsSystem::gravity.x;
	data["gravity"]["y"] = PhysicsSystem::gravity.y;
	data["gravity"]["z"] = PhysicsSystem::gravity.z;
	return data;
}

void PhysicsSystem::DeSerialize(const nlohmann::json& data)
{
	if (data.find("gravity") != data.end())
	{
		PhysicsSystem::gravity.x = data["gravity"]["x"];
		PhysicsSystem::gravity.y = data["gravity"]["y"];
		PhysicsSystem::gravity.z = data["gravity"]["z"];
	}
	else
	{
		PhysicsSystem::gravity = glm::vec3(0, -9.81, 0);
	}
}