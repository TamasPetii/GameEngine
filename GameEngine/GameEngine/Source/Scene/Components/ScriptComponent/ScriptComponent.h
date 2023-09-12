#pragma once
#include "../Component.h"
#include <windows.h>
#undef min
#undef max
#include <iostream>
#include <fstream>
#include <map>

class Script;
class Entity;
typedef Script* (*FunctionPointer)();

class ScriptComponent : public Component
{
public:
	static void LOAD_DLL();
	static void GENERATE_SCRIPT(const std::string& name);
	static HMODULE DLL_HANDLE;

	json SaveToJson() const override;
	void LoadFromJson(const json& object) override;
	ScriptComponent* Clone() const;

	ScriptComponent() {}
	ScriptComponent(const json& object);

	void OnStart();
	void OnUpdate(float deltaTime);
	void ReloadScripts(Entity* entity);
	Script* AttachScript(const std::string& name);
	void DeleteScripts();


	inline auto& Get_ScriptList() { return m_Scripts; }
private:
	std::map<std::string, Script*> m_Scripts;
};

inline HMODULE ScriptComponent::DLL_HANDLE;
