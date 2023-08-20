#pragma once
//#include "../../../Render/Entity/Entity.h"
#include <string>
#include <fstream>

class Entity;

class ScriptComponent
{
public:
	ScriptComponent(Entity* entity) : mEntity(entity) {}
	static void GenerateScript(const std::string& name);
	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDelete() = 0;
protected:
	Entity* mEntity;
private:
	static void GenerateHeader(const std::string& name);
	static void GenerateBody(const std::string& name);
};

