#pragma once
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Registry/System/System.h>
#include <Registry/Registry.h>
#include <Render/Renderer.h>
#include <Registry/Component/PointlightComponent.h>
#include <Manager/ResourceManager.h>


class PointLightSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
	static void OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<ResourceManager> manager);
};

