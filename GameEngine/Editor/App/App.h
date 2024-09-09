#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <memory>
#include <iostream>
#include <exception>
#include <algorithm>
#include <execution>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <Registry/Entity.h>
#include <Registry/Pool.h>
#include <Registry/Registry.h>
#include <Gui/Gui.h>
#include <Render/OpenGL/ProgramGL.h>
#include <Render/OpenGL/ShaderGL.h>
#include <Render/OpenGL/BufferGL.h>
#include <Render/OpenGL/ShaderStorageBufferGL.h>
#include <Render/OpenGL/UniformBufferGL.h>
#include <Registry/Component/TransformComponent.h>
#include <Render/Renderer.h>
#include "Scene/Scene.h"
#include <Settings/GlobalSettings.h>

class App
{
public:
	static App* Instance();
	static void Destroy();
	void Run();
private:
	static App* m_Instance;
	GLFWwindow* m_Window;
	App();
	~App();

	std::shared_ptr<Scene> m_Scene;
};