#include "ScriptApi.h"
#include <ImGui_Glfw/imgui.h>

void SetImGuiContext(void* context)
{
	if (context)
	{
		ImGui::SetCurrentContext(static_cast<ImGuiContext*>(context));
	}
}

/*

Script* CreateScript_BaseScript(std::shared_ptr<Registry> registry, Entity entity)
{
	return new BaseScript(registry, entity);
}

void BaseScript::OnStart()
{
}

void BaseScript::OnUpdate(float deltaTime)
{
}

void BaseScript::OnGui()
{
}

void BaseScript::OnTriggerEnter(Entity collidedEntity)
{
}

void BaseScript::OnTriggerExit(Entity collidedEntity)
{
}

void BaseScript::OnCollisionEnter(Entity collidedEntity)
{
}

void BaseScript::OnCollisionExit(Entity collidedEntity)
{
}

*/