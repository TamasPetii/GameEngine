#pragma once
#include "EngineApi.h"
#include "Registry/System/System.h"

class Registry;

class ENGINE_API InstanceSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
private:
	static void GeometryInstances(std::shared_ptr<Registry> registry);
	static void ModelInstances(std::shared_ptr<Registry> registry);
};

