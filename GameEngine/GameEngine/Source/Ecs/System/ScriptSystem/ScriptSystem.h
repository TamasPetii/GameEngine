#pragma once
#include <iostream>
#include <windows.h>
#undef min
#undef max

#include "../ISystem.h"

namespace Ecs
{
	class ScriptSystem : public ISystem
	{
	public:
		void LoadScriptDll();
		void GenerateScript(const std::string& name);
		HMODULE m_DllHandle;

		void OnStart(Entity* entity);
		void OnUpdate(Entity* entity);

		void ReloadScripts();
	};
}


