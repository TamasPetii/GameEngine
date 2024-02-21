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
		if (entity != nullptr && entity->HasComponent<ScriptComponent>())
		{
			auto scriptComponent = entity->GetComponent<ScriptComponent>();

			for (auto& [name, script] : scriptComponent->Ref_Scripts())
			{
				if (script != nullptr)
					script->OnStart();
			}
		}
	}

	void ScriptSystem::OnUpdate(Entity* entity, float deltaTime)
	{
		if (entity != nullptr && entity->HasComponent<ScriptComponent>())
		{
			auto scriptComponent = entity->GetComponent<ScriptComponent>();

			for (auto& [name, script] : scriptComponent->Ref_Scripts())
			{
				if(script != nullptr)
					script->OnUpdate(deltaTime);
			}
		}
	}

	void ScriptSystem::AttachScript(Entity* entity, const std::string& name)
	{
		if (entity != nullptr && entity->HasComponent<ScriptComponent>())
		{
			std::string functionName = "CreateScript_" + name;
			FunctionPointer createScript = (FunctionPointer)GetProcAddress(dllHandle, functionName.c_str());

			auto scriptList = entity->GetComponent<ScriptComponent>()->Ref_Scripts();

			if (createScript != nullptr)
			{
				scriptList[name] = createScript();
				scriptList[name]->AttachEntity(entity);
			}
			else
			{
				scriptList[name] = nullptr;
			}
		}
	}

	void ScriptSystem::DeleteScript(Entity* entity, const std::string& name)
	{
		if (entity != nullptr && entity->HasComponent<ScriptComponent>())
		{
			auto scriptList = entity->GetComponent<ScriptComponent>()->Ref_Scripts();

			if (scriptList.find(name) != scriptList.end() && scriptList[name] != nullptr)
			{
				delete scriptList[name];
			}

			scriptList.erase(name);
		}
	}

	void ScriptSystem::ClearScripts(Entity* entity)
	{
		if (entity != nullptr && entity->HasComponent<ScriptComponent>())
		{
			auto scriptList = entity->GetComponent<ScriptComponent>()->Ref_Scripts();

			for (auto& [name, script] : scriptList)
			{
				if (script != nullptr)
					delete script;
			}
		}
	}

	void ScriptSystem::ReloadScripts(Entity* entity)
	{
		if (entity != nullptr && entity->HasComponent<ScriptComponent>())
		{
			ScriptSystem::ClearScripts(entity);

			auto scriptList = entity->GetComponent<ScriptComponent>()->Ref_Scripts();
			
			for (auto& [name, script] : scriptList)
			{
				ScriptSystem::AttachScript(entity, name);
			}
		}
	}
}