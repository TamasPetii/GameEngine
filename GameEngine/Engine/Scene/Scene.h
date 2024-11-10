#pragma once
#include "EngineApi.h"
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/cooking/PxCooking.h>

#include <thread>
#include <chrono>
#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <unordered_map>

#include "Registry/Unique.h"
#include "Manager/Managers.h"
#include "Registry/Registry.h"
#include "Registry/System/Systems.h"
#include "Registry/Component/Components.h"

#include <glm/gtc/quaternion.hpp>
#include "Settings/GlobalSettings.h"
#include "Collision/CollisionCallback.h"

#include "Render/BloomRenderer.h"
#include "Render/SkyboxRenderer.h"


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
private:
	void UpdateSystemTime(float deltaTime);
	std::string name{"default_scene"};
	std::string path = "";
	std::shared_ptr<Registry> m_Registry;
	std::unordered_map<std::type_index, double> m_SystemTimes;
	std::unordered_map<std::type_index, double> m_AverageSystemTimes;
private:
	physx::PxDefaultAllocator gAllocator;
	physx::PxDefaultErrorCallback gErrorCallback;
	physx::PxFoundation* gFoundation = nullptr;
	physx::PxPhysics* gPhysics = nullptr;
	physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
	CollisionCallback* collisionCallback = nullptr;
public:
	physx::PxScene* gScene = nullptr;
};

