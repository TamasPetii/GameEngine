#pragma once
#include "../../JsonObject.h"

class Component : public JsonObject
{
public:
	virtual ~Component() = default;
	virtual Component* Clone() const = 0;
};

