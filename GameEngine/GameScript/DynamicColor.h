#pragma once
#include "Script.h"

class DLL_API DynamicColor : public Script
{
public:
	void OnStart() override;
	void OnUpdate(float deltaTime) override;
};

extern "C" DLL_API Script* CreateScript_DynamicColor()
{
	return new DynamicColor();
}
