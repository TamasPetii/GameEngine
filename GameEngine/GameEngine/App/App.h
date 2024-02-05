#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <exception>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Registry/Entity.h>
#include <Registry/Pool.h>
#include <Registry/Registry.h>

class App
{
public:
	static App* Instance();
	static void Destroy();
	void Run();
private:
	static App* m_Instance;
	App();
	~App();
	GLFWwindow* m_Window;
};

inline App* App::m_Instance = nullptr;