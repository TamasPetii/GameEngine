#include "ScriptComponent.h"
#include "../../../../../GameScript/Script.h"
#include "../../Entity/Entity.h"

void ScriptComponent::LOAD_DLL()
{
	FreeLibrary(DLL_HANDLE);

	system(R"(start /WAIT powershell -Command "& 'C:\Program Files\Microsoft Visual Studio\2022\Preview\MSBuild\Current\Bin\MSBuild.exe' 'D:\OpenGL\GameEngine\GameEngine\GameScript\GameScript.vcxproj' /t:Build /p:Platform=x64 /p:Configuration=Debug /p:SolutionDir='D:\OpenGL\GameEngine\GameEngine\'")");

	DLL_HANDLE = LoadLibrary(L"D:\\OpenGL\\GameEngine\\GameEngine\\x64\\Debug\\GameScript.dll");

	if (!DLL_HANDLE)
	{
		std::cerr << "Cannot open library: Error code " << GetLastError() << '\n';
		exit(1);
	}
}

void ScriptComponent::GENERATE_SCRIPT(const std::string& name)
{
	std::filesystem::path filepath = "D:\\OpenGL\\GameEngine\\GameEngine\\GameScript\\" + name + ".h";

	if (std::filesystem::exists(filepath))
	{
		std::cout << "Already Exists!" << std::endl;
		return;
	}

	std::ofstream header("D:\\OpenGL\\GameEngine\\GameEngine\\GameScript\\" + name + ".h");

	if (!header.is_open())
	{
		std::cerr << "Cannot open default script header" << '\n';
		exit(1);
	}

	header << R"(#pragma once)" << std::endl;
	header << R"(#include "Script.h")" << std::endl;
	header << std::endl;
	header << R"(class DLL_API )" + name + R"( : public Script)" << std::endl;
	header << "{" << std::endl;
	header << "public:" << std::endl;
	header << "\tvoid OnStart() override;" << std::endl;
	header << "\tvoid OnUpdate(float deltaTime) override;" << std::endl;
	header << "};" << std::endl;
	header << std::endl;
	header << R"(extern "C" DLL_API Script* CreateScript_)" + name + "()" << std::endl;
	header << "{" << std::endl;
	header << "\treturn new " + name + "();" << std::endl;
	header << "}" << std::endl;

	header.close();

	std::ofstream source("D:\\OpenGL\\GameEngine\\GameEngine\\GameScript\\" + name + ".cpp");

	if (!source.is_open())
	{
		std::cerr << "Cannot open default script source" << '\n';
		exit(1);
	}

	source << R"(#include ")" + name + R"(.h")" << std::endl;
	source << std::endl;
	source << "void " + name + "::OnStart()" << std::endl;
	source << "{" << std::endl;
	source << "}" << std::endl;
	source << std::endl;
	source << "void " + name + "::OnUpdate(float deltaTime)" << std::endl;
	source << "{" << std::endl;
	source << "}" << std::endl;

	source.close();
}

void ScriptComponent::ReloadScripts(Entity* entity)
{
	for (auto [name, script] : m_Scripts)
	{
		AttachScript(name, entity);
	}

	//TODO DELETE IF NULLPTR SCRIPT
	//Maybe callback with iterate?
}

void ScriptComponent::DeleteScripts()
{
	for (auto [name, script] : m_Scripts)
	{
		if (script)
			delete script;
		script = nullptr;
	}

}

void ScriptComponent::AttachScript(const std::string& name, Entity* entity)
{
	std::string functionName = "CreateScript_" + name;

	FunctionPointer createScript = (FunctionPointer)GetProcAddress(DLL_HANDLE, functionName.c_str());

	if (!createScript)
	{
		std::cerr << "Cannot load symbol: Error code " << GetLastError() << '\n';
		FreeLibrary(DLL_HANDLE);
		exit(1);
	}

	m_Scripts[name] = createScript();
	m_Scripts[name]->AttachEntity(entity);
}

void ScriptComponent::OnStart()
{ 
	for (auto [name, script] : m_Scripts)
	{
		script->OnStart();
	}
}

void ScriptComponent::OnUpdate(float deltaTime)
{
	for (auto [name, script] : m_Scripts)
	{
		script->OnUpdate(deltaTime);
	}
}


json ScriptComponent::SaveToJson() const
{
	return json();
}

void ScriptComponent::LoadFromJson(const json& object)
{
	return;
}

ScriptComponent* ScriptComponent::Clone() const
{
	return nullptr;
}