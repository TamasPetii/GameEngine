#pragma once
#include "EngineApi.h"
#include "Script.h"
#include <map>
#include <string>

class Registry;

typedef Script* (*FunctionPointer)(std::shared_ptr<Registry>, Entity);

struct ENGINE_API ScriptComponent
{
	std::map<std::string, Script*> scripts;
};