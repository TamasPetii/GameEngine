#pragma once
#include <iostream>
#include "BaseScript.h"
#include "Manager/ModelManager.h"

extern int asd;

class DLL_API MoveEntityScript : public BaseScript
{
public:
	void OnStart() override;
	void OnUpdate(float deltaTime) override;
};

extern "C" DLL_API BaseScript * CreateScript_MoveEntityScript()
{
	return new MoveEntityScript();
}
