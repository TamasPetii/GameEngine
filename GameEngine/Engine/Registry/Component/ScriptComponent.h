#pragma once
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

class BaseScript;
typedef BaseScript* (*FunctionPointer)();

class ScriptComponent : public Component
{
public:
	std::map<std::string, BaseScript*> scripts;
};