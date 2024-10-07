#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "BaseScript.h"
#include "Manager/ModelManager.h"

class SCRIPT_API ChangeAnimationScript : public BaseScript
{
public:
	ChangeAnimationScript(std::shared_ptr<Registry> registry, Entity entity) : BaseScript(registry, entity) {}
	~ChangeAnimationScript();
	void OnStart() override;
	void OnUpdate(float deltaTime) override;

	std::shared_ptr<irrklang::ISound> sound = nullptr;
	std::shared_ptr<irrklang::ISound> soundBg = nullptr;
};

extern "C" SCRIPT_API BaseScript * CreateScript_ChangeAnimationScript(std::shared_ptr<Registry> registry, Entity entity)
{
	return new ChangeAnimationScript(registry, entity);
}
