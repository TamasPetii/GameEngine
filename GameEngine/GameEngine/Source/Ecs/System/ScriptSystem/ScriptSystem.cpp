#include "ScriptSystem.h"

namespace Ecs
{
	void ScriptSystem::LoadScriptDll()
	{
		if (dllHandle != nullptr)
		{
			FreeLibrary(dllHandle);
		}

		//TODO: SCRIPT COMPILE

		dllHandle = LoadLibrary(L"GameScript.dll");

		if (dllHandle == nullptr)
		{
			//TODO SHOW ERROR MASSAGE ON UI
		}

	}

	void ScriptSystem::GenerateScript(const std::string& name)
	{
		// GENERATE WITH PYTHON
	}


	void ScriptSystem::OnStart(Entity* entity)
	{
		if (ComponentManager::HasComponent<ScriptComponent>(entity))
		{
			auto scriptComponent = ComponentManager::GetComponent<ScriptComponent>(entity);

			for (auto& [name, script] : scriptComponent->Ref_Scripts())
			{
				script->OnStart();
			}
		}
	}

	void ScriptSystem::OnUpdate(Entity* entity)
	{
		if (ComponentManager::HasComponent<ScriptComponent>(entity))
		{
			auto scriptComponent = ComponentManager::GetComponent<ScriptComponent>(entity);

			for (auto& [name, script] : scriptComponent->Ref_Scripts())
			{
				//script->OnUpdate();
			}
		}
	}

	void ScriptSystem::AttachScript(Entity* entity, const std::string& name)
	{
		if (ComponentManager::HasComponent<ScriptComponent>(entity))
		{
			std::string functionName = "CreateScript_" + name;
			FunctionPointer createScript = (FunctionPointer)GetProcAddress(m_DllHandle, functionName.c_str());

			auto scriptList = ComponentManager::GetComponent<ScriptComponent>(entity)->Ref_Scripts();
			scriptList[name] = createScript != nullptr ? createScript() : nullptr;
		}
	}

	void ScriptSystem::DeleteScript(Entity* entity, const std::string& name)
	{
		if (ComponentManager::HasComponent<ScriptComponent>(entity))
		{
			auto scriptList = ComponentManager::GetComponent<ScriptComponent>(entity)->Ref_Scripts();

			if (scriptList.find(name) != scriptList.end() && scriptList[name] != nullptr)
			{
				delete scriptList[name];
			}

			scriptList.erase(name);
		}
	}

	void ScriptSystem::ClearScripts(Entity* entity)
	{
		if (ComponentManager::HasComponent<ScriptComponent>(entity))
		{
			auto scriptList = ComponentManager::GetComponent<ScriptComponent>(entity)->Ref_Scripts();

			for (auto& [name, script] : scriptList)
			{
				if (script != nullptr)
					delete script;
			}
		}
	}

	void ScriptSystem::ReloadScripts(Entity* entity)
	{
		if (ComponentManager::HasComponent<ScriptComponent>(entity))
		{
			ScriptSystem::ClearScripts(entity);

			auto scriptList = ComponentManager::GetComponent<ScriptComponent>(entity)->Ref_Scripts();
			
			for (auto& [name, script] : scriptList)
			{
				ScriptSystem::AttachScript(entity, name);
			}
		}
	}
}