#pragma once
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>
#include <IMGUI/ImGuizmo.h>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/matrix_decompose.hpp>
#include "../Render/Renderer.h"

class Interface
{
public:
	Interface(GLFWwindow* window, Renderer* renderer);
	void Render();
	void Update();
private:
	void PreRender();
	void PostRender();
	void RenderDockSpace();
	void RenderViewPortWindow();
	void RenderGizmos();
	void RenderGameObjectWindow();
	void RenderComponentsWindow();
	void RenderSettingsWindow();
	void Camera_KeyboardEvent();
	void Camera_MouseEvent();
	GLFWwindow* mWindow;
	Renderer* mRenderer;
	ImVec2 mViewPortClickCoords;
	ImVec2 mViewPortSize;
	bool mViewPortResize;
};

