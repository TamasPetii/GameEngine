#pragma once
#include "EngineApi.h"
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Registry/Registry.h"
#include "Render/OpenGL/Camera.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/SpotLightComponent.h"

class ENGINE_API SpotLightSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
};
