#pragma once
#include <memory>

#include "EngineApi.h"
#include "Settings/GlobalSettings.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Manager/TextureManager.h"
#include "Registry/System/CameraSystem.h"

class ENGINE_API SkyboxRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry);
	static nlohmann::json Serialize();
	static void DeSerialize(const nlohmann::json& data);
public:
	static bool UseSkybox;
	static glm::vec3 SkyboxRotation;
	static glm::vec3 SkyboxRotationSpeed;
	static glm::ivec3 SkyboxRotationDirection;
	static glm::mat4 SkyboxRotationMatrix;
	static std::shared_ptr<TextureGL> SkyboxTexture;
};

