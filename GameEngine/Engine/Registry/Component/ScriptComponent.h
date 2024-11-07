#pragma once
#include "EngineApi.h"
#include "Script.h"

#include <map>
#include <string>

typedef Script* (*FunctionPointer)(std::shared_ptr<Registry>, Entity);

class ENGINE_API ScriptComponent
{
public:
	std::map<std::string, Script*> scripts;
};