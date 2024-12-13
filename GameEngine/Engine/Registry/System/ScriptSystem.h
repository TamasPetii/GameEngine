#pragma once
#include "EngineApi.h"
#include "Registry/System/System.h"
#include "Registry/Entity.h"
#include <windows.h>

class Registry;
typedef void (*ImGuiContextFunction)(void*);

class ENGINE_API ScriptSystem : public System
{
public:
	static void FreeLib();
	static void LoadLib();
	static void FreeScripts(std::shared_ptr<Registry> registry);
	static void LoadScripts(std::shared_ptr<Registry> registry);

	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry, float deltaTime);
	static void OnEnd(std::shared_ptr<Registry> registry);

	static nlohmann::json Serialize(Registry* registry, Entity entity);
	static void DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data);
	static ImGuiContextFunction SetImGuiContextFunction;
private:
	static bool DLL_CHANGED;
	static HMODULE DLL_HANDLE;
};
