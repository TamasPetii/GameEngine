#pragma once
#include <chrono>
#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <unordered_map>

#include "EngineApi.h"
#include "Registry/Unique.h"
#include "Manager/Managers.h"
#include "Registry/Registry.h"
#include "Render/OpenGL/Camera.h"
#include "Registry/System/Systems.h"
#include "Registry/Component/Components.h"

#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/cooking/PxCooking.h>
#include <glm/gtc/quaternion.hpp>

class ENGINE_API Scene
{
public:
	Scene();
	~Scene();
	void Update(float deltaTime);
	auto& GetRegistry() { return m_Registry; }
	auto& GetMainCamera() { return m_SceneCamera; }
	auto& GetSystemTimes() { return m_AverageSystemTimes; }
	void Serialize(const std::string& path);
	void DeSerialize(const std::string& path);
private:
	void UpdateCamera(float deltaTime);
	void UpdateSystemTime(float deltaTime);
	std::string name{"default_scene"};
	std::shared_ptr<Registry> m_Registry;
	std::shared_ptr<Camera> m_SceneCamera;
	std::unordered_map<std::type_index, double> m_SystemTimes;
	std::unordered_map<std::type_index, double> m_AverageSystemTimes;


	physx::PxDefaultAllocator gAllocator;
	physx::PxDefaultErrorCallback gErrorCallback;
	physx::PxFoundation* gFoundation = nullptr;
	physx::PxPhysics* gPhysics = nullptr;
	physx::PxScene* gScene = nullptr;
	physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
};
