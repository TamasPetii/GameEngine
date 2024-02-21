#pragma once
<<<<<<< HEAD
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "System.h"
#include "Registry/Registry.h"
#include "Registry/Component/TransformComponent.h"
#include "Manager/ResourceManager.h"
=======
#include <Registry/System/System.h>
#include <Registry/Registry.h>
#include <Render/Renderer.h>
#include <Registry/Component/TransformComponent.h>
#include <Manager/ResourceManager.h>
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

class TransformSystem : public System
{
public:
<<<<<<< HEAD
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
	static void OnEnd(std::shared_ptr<Registry> registry);
=======
	static void OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
	static void OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
	static void OnEnd(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
};

