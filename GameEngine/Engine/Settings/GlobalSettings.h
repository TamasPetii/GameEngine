#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>
#include "Manager/TextureManager.h"
#include <array>

struct ENGINE_API GlobalSettings
{
	static float bias;

	static bool Inited;
	static bool HideCursor;
	static bool GameViewActive;
	static bool UseDockingSpace;
	static bool EnablePhysicsInEditor;
	static bool IsViewPortActive;
	static bool DeployedGame;
	static bool SimulateScene; 

	static std::string ProjectPath;
	static std::string CompilerPath;
	static std::string DefaultEnginePath;

	static bool UseLod;
	static std::array<float, 4> LodDistances;
};

