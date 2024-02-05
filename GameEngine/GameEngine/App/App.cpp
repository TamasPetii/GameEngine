#include "App.h"

App::App()
{
	if (!glfwInit())
		throw std::runtime_error("[Application::Init()] : Failed to initialize GLFW.\n(" + std::to_string(__LINE__) + "," + __FILE__ + ")");

	glfwWindowHint(GLFW_RESIZABLE, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(1024, 768, "GameEngine", NULL, NULL);
	if (!m_Window)
		throw std::runtime_error("[Application::Init()] : Failed to initialize GLFW window.\n(" + std::to_string(__LINE__) + "," + __FILE__ + ")");

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(0);

	if (glewInit() != GLEW_OK)
		throw std::runtime_error("[Application::Init()] : Failed to initialize GLEW. (" + std::to_string(__LINE__) + "," + __FILE__ + ")");

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

	std::string title = "GameEngine | OpenGL " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	glfwSetWindowTitle(m_Window, title.c_str());
}

App::~App()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

App* App::Instance()
{
	if (m_Instance == nullptr)
		m_Instance = new App();

	return m_Instance;
}

void App::Destroy()
{
	if (m_Instance != nullptr)
		delete m_Instance;
}

void App::Run()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}
}