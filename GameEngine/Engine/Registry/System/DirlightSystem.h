#pragma once
#include "EngineApi.h"
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "System.h"
#include "Registry/Registry.h"
#include "Registry/Component/DirLightComponent.h"
#include "Manager/ResourceManager.h"
#include "Render/OpenGL/Camera.h"

class ENGINE_API DirlightSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry, std::shared_ptr<Camera> camera);
	static void OnUpdate(std::shared_ptr<Registry> registry, std::shared_ptr<Camera> camera);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};

