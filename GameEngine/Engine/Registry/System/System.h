#pragma once
#include "EngineApi.h"
#include <nlohmann/json.hpp>
#include <memory>

class ENGINE_API System
{
public:
	virtual ~System() = default;
};