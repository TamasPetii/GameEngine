#include "App.h"

App::App()
{
	mInterface = nullptr;
	mRenderer = nullptr;
	mWindow = nullptr;
	mWindowWidth = 1600;
	mWindowHeight = 900;
}

void App::Init()
{
	/*Initialize GLFW */
	if (!glfwInit())
	{
		throw std::runtime_error("GLFW Initialization Error");
	}

	/* Window Predefinitions */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create Window */
	mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "Game Engine", NULL, NULL);
	if (!mWindow)
	{
		throw std::runtime_error("GLFW Window Initialization Error");
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(mWindow);

	/* VSYNC */
	glfwSwapInterval(0);

	/* Initialize GLEW */
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("GLEW Initialization Error");
	}

	mRenderer = new Renderer();
	mInterface = new Interface(mWindow, mRenderer);
}

void App::Run()
{
	/* Game Loop */
	while (!glfwWindowShouldClose(mWindow))
	{
		glfwPollEvents();

		mRenderer->Update();
		mInterface->Update();
		mRenderer->Render();
		mInterface->Render();

		glfwSwapBuffers(mWindow);
	}
}

void App::Clear()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void App::window_size_callback(GLFWwindow* window, int width, int height)
{
	mRenderer->Update();
	mInterface->Update();
	mRenderer->Render();
	mInterface->Render();
	glfwSwapBuffers(window);
}