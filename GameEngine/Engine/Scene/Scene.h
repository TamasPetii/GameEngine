#pragma once
#include "EngineApi.h"
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/cooking/PxCooking.h>
using namespace physx;

#include <string>
#include <memory>
#include <typeindex>
#include <unordered_map>

class Registry;
class CollisionCallback;

class ENGINE_API Scene
{
public:
	Scene();
	~Scene();
	void Update(float deltaTime);
	auto& GetRegistry() { return m_Registry; }
	auto& GetSystemTimes() { return m_AverageSystemTimes; }
	auto& GetPath() { return path; }
	void Serialize(const std::string& path);
	void DeSerialize(const std::string& path);
	void StartGame();
	void EndGame();
private:
	void UpdateSystemTime(float deltaTime);
	std::string name{"default_scene"};
	std::string path = "";
	std::shared_ptr<Registry> m_Registry;
	std::unordered_map<std::type_index, double> m_SystemTimes;
	std::unordered_map<std::type_index, double> m_AverageSystemTimes;
private:
	bool physicsSimulationFetchInit = true;
	PxDefaultAllocator gAllocator;
	PxDefaultErrorCallback gErrorCallback;
	PxFoundation* gFoundation = nullptr;
	PxPhysics* gPhysics = nullptr;
	PxDefaultCpuDispatcher* gDispatcher = nullptr;
	CollisionCallback* collisionCallback = nullptr;
public:
	PxScene* gScene = nullptr;
};

