#include "ScriptApi.h"
#include <ImGui_Glfw/imgui.h>

void SetImGuiContext(void* context)
{
	if (context)
	{
		ImGui::SetCurrentContext(static_cast<ImGuiContext*>(context));
	}
}