#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>
#include "Manager/TextureManager.h"

struct ENGINE_API GlobalSettings
{
	static bool GameViewActive;
	static bool UseDockingSpace;
	static bool EnablePhysicsInEditor;

	static bool UseSkybox;
	static std::shared_ptr<TextureGL> SkyboxTexture;
};

