#include "Application.h"
Application* Application::s_Instance = nullptr;

Application::Application(ApplicationSpecification spec)
	: m_Specification(spec), m_Window(nullptr)
{
	m_Window = InitWindow();
	
	if (!m_Window)
	{
		std::cout << "[Application Error]: Cannot load GLFW window." << std::endl;
		return;
	}

	s_Instance = this;

	InitImGui();

	// blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	// depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

Application::~Application()
{
	for (auto& layer : m_Layers)
		layer->OnDetach();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void Application::Run()
{
	if (!m_Window)
	{
		std::cout << "[Application Error]: Cannot run application, there is no window." << std::endl;
		return;
	}

	auto start = std::chrono::steady_clock::now();
	auto end = std::chrono::steady_clock::now();

	do {
		// update delta time
		end = std::chrono::steady_clock::now();
		m_DeltaTime = float(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000000.f;
		start = std::chrono::steady_clock::now();

		// clear framebuffers
		Renderer::Clear();

		// update
		for (auto& layer : m_Layers)
		{
			if (layer->IsVisible()) layer->OnUpdate(m_DeltaTime);
		}

		// prepare next imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// update imgui
		for (auto& layer : m_Layers)
		{
			if (layer->IsVisible()) layer->OnImGuiRender();
		}

		// render imgui 
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// swap buffers
		glfwSwapBuffers(m_Window);
		glfwPollEvents();

	} while (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(m_Window) == 0);
}

Application& Application::Get()
{
	return *s_Instance;
}

GLFWwindow* Application::GetWindowHandle()
{
	return Get().m_Window;
}

GLFWwindow* Application::InitWindow()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return nullptr;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(
		m_Specification.WindowSize.x, m_Specification.WindowSize.y, 
		m_Specification.Name.c_str(), NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version\n");
		glfwTerminate();
		return nullptr;

	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return nullptr;
	}

	std::cout << "Using GL Version: " << glGetString(GL_VERSION) << std::endl;

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	return window;
}

ApplicationSpecification Application::GetSpecification()
{
	return Get().m_Specification;
}

void Application::InitImGui()
{
	// imgui initialization
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}