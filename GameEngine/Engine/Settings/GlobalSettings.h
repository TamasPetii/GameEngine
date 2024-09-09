#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>
#include "Manager/TextureManager.h"

struct ENGINE_API GlobalSettings
{
	static float bias;

	static bool HideCursor;
	static bool GameViewActive;
	static bool UseDockingSpace;
	static bool EnablePhysicsInEditor;
	static bool EnableAnimationInEditor;

	static bool UseSkybox;
	static glm::vec3 SkyboxRotation;
	static glm::vec3 SkyboxRotationSpeed;
	static glm::ivec3 SkyboxRotationDirection;
	static std::shared_ptr<TextureGL> SkyboxTexture;
	static glm::mat4 SkyboxRotationMatrix;
};

