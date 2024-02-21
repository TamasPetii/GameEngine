#pragma once
#include <chrono>
#include <vector>
#include <memory>
<<<<<<< HEAD
#include <random>
#include <iostream>
#include <unordered_map>

#include "Registry/Unique.h"
#include "Manager/Managers.h"
#include "Registry/Registry.h"
#include "Render/OpenGL/Camera.h"
#include "Registry/System/Systems.h"
#include "Registry/Component/Components.h"
=======
#include <iostream>

#include <Registry/Registry.h>
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

class Scene
{
public:
	Scene();
	~Scene();
<<<<<<< HEAD
	void Update(float deltaTime);
	auto& GetRegistry() { return m_Registry; }
	auto& GetMainCamera() { return m_SceneCamera; }
	auto& GetSystemTimes() { return m_AverageSystemTimes; }
private:
	void UpdateCamera(float deltaTime);
	void UpdateSystemTime(float deltaTime);
	std::string name;
	std::shared_ptr<Registry> m_Registry;
	std::shared_ptr<Camera> m_SceneCamera;
	std::unordered_map<UniqueID, double> m_SystemTimes;
	std::unordered_map<UniqueID, double> m_AverageSystemTimes;
=======
private:
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
};

