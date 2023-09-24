#include "ScriptSystem.h"

namespace Ecs
{
	void ScriptSystem::LoadScriptDll()
	{
		if (m_DllHandle != nullptr)
		{
			FreeLibrary(m_DllHandle);
		}

		//TODO: SCRIPT COMPILE

		m_DllHandle = LoadLibrary(L"GameScript.dll");

		if (m_DllHandle == nullptr)
		{
			//TODO SHOW ERROR MASSAGE ON UI
		}

	}
	void GenerateScript(const std::string& name);
	HMODULE m_DllHandle;
}