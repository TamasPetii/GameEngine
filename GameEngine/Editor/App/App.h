#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Scene/Scene.h"

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