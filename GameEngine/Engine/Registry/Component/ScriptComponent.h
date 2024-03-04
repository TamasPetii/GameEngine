#pragma once
#include "EngineApi.h"
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

class BaseScript;
typedef BaseScript* (*FunctionPointer)();

class ENGINE_API ScriptComponent : public Component
{
public:
	std::map<std::string, BaseScript*> scripts;
};