#pragma once
#include "EngineApi.h"
#include <windows.h>
#include <algorithm>
#include <execution>

#include "System.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/ScriptComponent.h"

class ENGINE_API ScriptSystem : public System
{
public:
	static void LoadScript(std::shared_ptr<Registry> registry);
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry, float deltaTime);
	static void OnEnd(std::shared_ptr<Registry> registry);
private:
	static bool DLL_CHANGED;
	static HMODULE DLL_HANDLE;
};
