#pragma once
#include "Script.h"

class DLL_API TestScript : public Script
{
public:
	void OnStart() override;
	void OnUpdate(float deltaTime) override;
};

extern "C" DLL_API Script * CreateScript_TestScript()
{
	return new TestScript();
}