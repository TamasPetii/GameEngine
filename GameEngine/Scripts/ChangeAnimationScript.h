#pragma once
#include "ScriptApi.h"

#include <iostream>
#include <glm/glm.hpp>
#include "Manager/ModelManager.h"
#include "Manager/InputManager.h"
#include "Registry/Component/AudioComponent.h"

class SCRIPT_API ChangeAnimationScript : public Script
{
public:
	ChangeAnimationScript(std::shared_ptr<Registry> registry, Entity entity) : Script(registry, entity) {}
	~ChangeAnimationScript();
	void OnStart() override;
	void OnUpdate(float deltaTime) override;

	std::shared_ptr<irrklang::ISound> sound = nullptr;
	std::shared_ptr<irrklang::ISound> soundBg = nullptr;
};

extern "C" SCRIPT_API Script* CreateScript_ChangeAnimationScript(std::shared_ptr<Registry> registry, Entity entity)
{
	return new ChangeAnimationScript(registry, entity);
}
