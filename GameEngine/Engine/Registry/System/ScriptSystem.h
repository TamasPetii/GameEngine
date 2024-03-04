#pragma once
#include <windows.h>
#include <algorithm>
#include <execution>

#include "System.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/ScriptComponent.h"

class ScriptSystem : public System
{
public:
	static void LoadScript(std::shared_ptr<Registry> registry);
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry, float deltaTime);
	static void OnEnd(std::shared_ptr<Registry> registry);
private:
	static inline bool DLL_CHANGED = false;
	static inline HMODULE DLL_HANDLE = NULL;
};
