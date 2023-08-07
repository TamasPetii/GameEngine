#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include "../Interface/Interface.h"
#include "../Render/Renderer.h"

class App
{
public:
	App();
	void Init();
	void Run();
	void Clear();
private:
	void window_size_callback(GLFWwindow* window, int width, int height);
	Interface* mInterface;
	Renderer* mRenderer;
	GLFWwindow* mWindow;
	unsigned int mWindowWidth;
	unsigned int mWindowHeight;
};

