#include "ScriptSystem.h"
#include "../Scripts/BaseScript.h"

void ScriptSystem::LoadScript(std::shared_ptr<Registry> registry)
{
	FreeLibrary(DLL_HANDLE);

	//system(R"(start /WAIT powershell -Command "& 'C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe' 'C:\Users\User\Desktop\GameEngine\GameEngine\Scripts\Scripts.vcxproj' /t:Build /p:Platform=x64 /p:Configuration=Release /p:SolutionDir='C:\Users\User\Desktop\GameEngine\GameEngine\'")");

	DLL_HANDLE = LoadLibrary("C:\\Users\\User\\Desktop\\GameEngine\\GameEngine\\x64\\Release\\Scripts.dll");

	if (!DLL_HANDLE)
	{
		std::cerr << "Cannot open library: Error code " << GetLastError() << '\n';
		exit(1);
	}

	auto scriptPool = registry->GetComponentPool<ScriptComponent>();
	std::for_each(std::execution::seq, scriptPool->GetDenseEntitiesArray().begin(), scriptPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			auto& scriptComponent = scriptPool->GetComponent(entity);
			auto index = scriptPool->GetIndex(entity);

			for (auto& [name, script] : scriptComponent.scripts)
			{
				if(script != nullptr)
					delete script;

				std::string functionName = "CreateScript_" + name;
				FunctionPointer createScript = (FunctionPointer)GetProcAddress(DLL_HANDLE, functionName.c_str());

				if (!createScript)
				{
					std::cerr << "Cannot load symbol: Error code " << GetLastError() << '\n';
					script = nullptr;
				}
				else
				{
					script = createScript(registry, entity);
				}
			}
		}
	);
}

void ScriptSystem::OnStart(std::shared_ptr<Registry> registry)
{

}

void ScriptSystem::OnUpdate(std::shared_ptr<Registry> registry, float deltaTime)
{
	auto resourceManager = ResourceManager::Instance();
	auto scriptPool = registry->GetComponentPool<ScriptComponent>();

	std::for_each(std::execution::seq, scriptPool->GetDenseEntitiesArray().begin(), scriptPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			auto& scriptComponent = scriptPool->GetComponent(entity);
			auto index = scriptPool->GetIndex(entity);

			for (auto& [name, script] : scriptComponent.scripts)
			{
				if(script != nullptr)
					script->OnUpdate(deltaTime);
			}
		}
	);
}

void ScriptSystem::OnEnd(std::shared_ptr<Registry> registry)
{
}