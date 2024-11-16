#pragma once
#pragma warning(disable: 4251)
#define _USE_MATH_DEFINES 

#ifdef EXPORT_SCRIPT_DLL
#define SCRIPT_API __declspec(dllexport)
#else
#define SCRIPT_API __declspec(dllimport)
#endif

#include <GL/glew.h>
#include "Registry/Component/Script.h"

extern "C" SCRIPT_API void SetImGuiContext(void* context);

/*
class SCRIPT_API BaseScript : public Script
{
public:
	BaseScript(std::shared_ptr<Registry> registry, Entity entity) : Script(registry, entity) {}

	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	void OnGui() override;
	void OnTriggerEnter(Entity collidedEntity) override;
	void OnTriggerExit(Entity collidedEntity) override;
	void OnCollisionEnter(Entity collidedEntity) override;
	void OnCollisionExit(Entity collidedEntity) override;
};

extern "C" SCRIPT_API Script* CreateScript_BaseScript(std::shared_ptr<Registry> registry, Entity entity);
*/