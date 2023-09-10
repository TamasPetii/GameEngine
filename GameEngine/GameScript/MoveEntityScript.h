#pragma once
#include "Script.h"

class DLL_API MoveEntityScript : public Script
{
public:
	void OnStart() override;
	void OnUpdate(float deltaTime) override;
};

extern "C" DLL_API Script* CreateScript_MoveEntityScript()
{
	return new MoveEntityScript();
}
