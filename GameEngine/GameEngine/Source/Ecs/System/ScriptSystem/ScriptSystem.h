#pragma once
#include "../ISystem.h"
#include "../../../../../GameScript/Script.h"

#include <iostream>
#include <windows.h>
#undef min
#undef max

typedef Script* (*FunctionPointer)();

namespace Ecs
{
	class ScriptSystem : public ISystem
	{
	public:
		static void LoadScriptDll();
		static void GenerateScript(const std::string& name);
		static HMODULE dllHandle;

		static void OnStart(Entity* entity);
		static void OnUpdate(Entity* entity);

		static void ReloadScripts(Entity* entity);
		static void ClearScripts(Entity* entity);
		static void AttachScript(Entity* entity, const std::string& name);
		static void DeleteScript(Entity* entity, const std::string& name);
	};
}

inline HMODULE Ecs::ScriptSystem::dllHandle;


