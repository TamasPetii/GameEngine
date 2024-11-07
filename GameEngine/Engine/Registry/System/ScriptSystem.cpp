#include "ScriptSystem.h"

bool ScriptSystem::DLL_CHANGED = false;
HMODULE ScriptSystem::DLL_HANDLE = NULL;

void ScriptSystem::FreeScripts(std::shared_ptr<Registry> registry)
{
	auto scriptPool = registry->GetComponentPool<ScriptComponent>();

	if (!scriptPool)
		return;

	std::for_each(std::execution::seq, scriptPool->GetDenseEntitiesArray().begin(), scriptPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			auto& scriptComponent = scriptPool->GetComponent(entity);

			for (auto& [name, script] : scriptComponent.scripts)
			{
				if (script != nullptr)
				{
					delete script;
					script = nullptr;
				}
			}
		}
	);

	FreeLibrary(DLL_HANDLE);
}

void ScriptSystem::LoadScripts(std::shared_ptr<Registry> registry)
{
	FreeScripts(registry);

	std::string compilerPath = GlobalSettings::CompilerPath;
	std::string projectPath = GlobalSettings::ProjectPath;
	std::string scriptSolutionPath = projectPath + "\\Scripts\\";
	std::string scriptVcxprojPath = scriptSolutionPath + "Scripts\\Scripts.vcxproj";
	std::string platform = "x64";

#ifdef _DEBUG
	std::string configuration = "Debug";
	std::string scriptPath = scriptSolutionPath + "x64\\Debug\\Scripts.dll";
#else
	std::string configuration = "Release";
	std::string scriptPath = scriptSolutionPath + "x64\\Release\\Scripts.dll";
#endif

	// Construct the command string
	std::string command = "start /WAIT powershell -Command \"& '";
	command += compilerPath + "' '";
	command += scriptVcxprojPath + "' /t:Build /p:Platform=" + platform;
	command += " /p:Configuration=" + configuration;
	command += " /p:SolutionDir='" + scriptSolutionPath + "'\"";

	int result = system(command.c_str());

	if (result != 0) {
		LOG_ERROR("ScriptSystem", "Building script failed");
	}

	DLL_HANDLE = LoadLibrary(scriptPath.c_str());

	if (!DLL_HANDLE)
	{
		LOG_ERROR("ScriptSystem", "Loading script dll failed.");
		return;
	}

	auto scriptPool = registry->GetComponentPool<ScriptComponent>();

	if (!scriptPool)
		return;

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
	auto resourceManager = ResourceManager::Instance();
	auto scriptPool = registry->GetComponentPool<ScriptComponent>();

	if (!scriptPool)
		return;

	std::for_each(std::execution::seq, scriptPool->GetDenseEntitiesArray().begin(), scriptPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			auto& scriptComponent = scriptPool->GetComponent(entity);
			auto index = scriptPool->GetIndex(entity);

			for (auto& [name, script] : scriptComponent.scripts)
			{
				if (script != nullptr)
				{
					script->OnStart();
				}
			}
		}
	);
}

void ScriptSystem::OnUpdate(std::shared_ptr<Registry> registry, float deltaTime)
{
	auto resourceManager = ResourceManager::Instance();
	auto scriptPool = registry->GetComponentPool<ScriptComponent>();

	if (!scriptPool)
		return;

	std::for_each(std::execution::seq, scriptPool->GetDenseEntitiesArray().begin(), scriptPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			auto& scriptComponent = scriptPool->GetComponent(entity);
			auto index = scriptPool->GetIndex(entity);

			for (auto& [name, script] : scriptComponent.scripts)
			{
				if (script != nullptr)
				{
					script->OnUpdate(deltaTime);
				}
			}
		}
	);
}

void ScriptSystem::OnEnd(std::shared_ptr<Registry> registry)
{
}

nlohmann::json ScriptSystem::Serialize(Registry* registry, Entity entity)
{
	auto& scriptComponent = registry->GetComponent<ScriptComponent>(entity);

	nlohmann::json data;
	data["scripts"] = nlohmann::json::array();

	for (auto& [name, script] : scriptComponent.scripts)
		data["scripts"].push_back(name);

	return data;
}

void ScriptSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& scriptComponent = registry->GetComponent<ScriptComponent>(entity);

	for (auto& name : data["scripts"])
		scriptComponent.scripts[name] = nullptr;

	registry->SetFlag<ScriptComponent>(entity, UPDATE_FLAG);
}