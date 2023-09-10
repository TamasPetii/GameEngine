#pragma once
#include "Script.h"

class DLL_API LightMoveScript : public Script
{
public:
	void OnStart() override;
	void OnUpdate(float deltaTime) override;
};

extern "C" DLL_API Script* CreateScript_LightMoveScript()
{
	return new LightMoveScript();
}
