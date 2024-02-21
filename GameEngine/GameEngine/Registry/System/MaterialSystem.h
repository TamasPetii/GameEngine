#pragma once
<<<<<<< HEAD
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>

#include "System.h"
#include "Registry/Registry.h"
#include "Registry/Component/MaterialComponent.h"
#include "Manager/ResourceManager.h"
=======
#include <Registry/System/System.h>
#include <Registry/Registry.h>
#include <Registry/Component/MaterialComponent.h>
#include <Render/Renderer.h>
#include <chrono>
#include <Manager/ResourceManager.h>
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

class MaterialSystem : public System
{
public:
<<<<<<< HEAD
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
=======
	static void OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
	static void OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
};

