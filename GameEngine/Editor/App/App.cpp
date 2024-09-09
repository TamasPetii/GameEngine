#include "App.h"

App* App::m_Instance = nullptr;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

	if (action == GLFW_PRESS)
	{
		InputManager::Instance()->SetKeyboardKey(key, true);
	}
	else if (action == GLFW_RELEASE)
	{
		InputManager::Instance()->SetKeyboardKey(key, false);
	}
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

	if (action == GLFW_PRESS)
	{
		InputManager::Instance()->SetMouseButton(button, true);
	}
	else if (action == GLFW_RELEASE)
	{
		InputManager::Instance()->SetMouseButton(button, false);
	}
}

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

	InputManager::Instance()->SetMousePosition(xpos, ypos);
}

App::App()
{
	if (!glfwInit())
		throw std::runtime_error("[Application::Init()] : Failed to initialize GLFW.\n(" + std::to_string(__LINE__) + "," + __FILE__ + ")");

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
	const int32_t width = static_cast<int32_t>(videoMode->width * 0.9f);
	const int32_t height = static_cast<int32_t>(videoMode->height * 0.9f);

	glfwWindowHint(GLFW_RESIZABLE, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(width, height, "GameEngine", NULL, NULL);
	if (!m_Window)
		throw std::runtime_error("[Application::Init()] : Failed to initialize GLFW window.\n(" + std::to_string(__LINE__) + "," + __FILE__ + ")");

	const int32_t windowLeft = videoMode->width / 2 - width / 2;
	const int32_t windowTop = videoMode->height / 2 - height / 2;
	glfwSetWindowPos(m_Window, windowLeft, windowTop);
	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(0);

	if (glewInit() != GLEW_OK)
		throw std::runtime_error("[Application::Init()] : Failed to initialize GLEW. (" + std::to_string(__LINE__) + "," + __FILE__ + ")");

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

	std::string title = "GameEngine | OpenGL " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	glfwSetWindowTitle(m_Window, title.c_str());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
	Gui::SetStyle();

	io.Fonts->AddFontDefault();
	ImFontConfig config;
	config.MergeMode = true;
	config.GlyphOffset.y = 2.f;
	config.GlyphMinAdvanceX = 16.0f;
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	io.Fonts->AddFontFromFileTTF("../Assets/Fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);

	glfwSetKeyCallback(m_Window, KeyCallback);
	glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
	glfwSetCursorPosCallback(m_Window, CursorPositionCallback);

	m_Scene = std::make_shared<Scene>();
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

		static float lastTime = static_cast<float>(glfwGetTime());
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		/*
		static bool cursorState = true;
		if (!GlobalSettings::HideCursor && cursorState != GlobalSettings::HideCursor)
		{
			cursorState = GlobalSettings::HideCursor;
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else if (GlobalSettings::HideCursor && cursorState != GlobalSettings::HideCursor)
		{
			cursorState = GlobalSettings::HideCursor;
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		*/

		m_Scene->Update(deltaTime);
		Gui::Update(m_Scene);
		Renderer::RenderScene(m_Scene, deltaTime);
		Gui::Render(m_Scene, deltaTime);

		InputManager::Instance()->UpdatePrevious();

		glfwSwapBuffers(m_Window);
	}
}