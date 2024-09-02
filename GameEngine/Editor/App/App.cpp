#include "App.h"

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

	GLint maxUboSize;
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUboSize);
	std::cout << "Maximum Uniform Block Size: " << maxUboSize << " bytes" << std::endl;

	GLint maxSsboBlockSize;
	glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxSsboBlockSize);
	std::cout << "Maximum Shader Storage Block Size: " << maxSsboBlockSize << " bytes" << std::endl;


	// Check for GL_ARB_bindless_texture
	if (glewIsSupported("GL_ARB_bindless_texture"))
	{
		std::cout << "GL_ARB_bindless_texture is supported!" << std::endl;
	}
	else
	{
		std::cout << "GL_ARB_bindless_texture is not supported." << std::endl;
	}

	// Check for GL_ARB_direct_state_access
	if (glewIsSupported("GL_ARB_direct_state_access"))
	{
		std::cout << "GL_ARB_direct_state_access is supported!" << std::endl;
	}
	else
	{
		std::cout << "GL_ARB_direct_state_access is not supported." << std::endl;
	}

	if (glfwExtensionSupported("GL_ARB_buffer_storage"))
		std::cout << "Buffer Storage Supported" << std::endl;

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

		m_Scene->Update(deltaTime);
		Gui::Update(m_Scene);
		Renderer::RenderScene(m_Scene, deltaTime);
		Gui::Render(m_Scene, deltaTime);

		glfwSwapBuffers(m_Window);
	}
}