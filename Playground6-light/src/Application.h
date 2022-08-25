#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Debug.h"
#include "Layer.h"
#include "Renderer.h"

struct ApplicationSpecification
{
	std::string Name;
	glm::ivec2 WindowSize;
};

class Application
{
public:
	Application(ApplicationSpecification spec);
	virtual ~Application();
	
	void Run();

	template<typename T>
	void PushLayer()
	{
		static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
		m_Layers.emplace_back(std::make_shared<T>());
		m_Layers.back()->OnAttach();
	}

	static Application& Get();
	static GLFWwindow* GetWindowHandle();
	static ApplicationSpecification GetSpecification();

private:
	GLFWwindow* InitWindow();
	void InitImGui();

private:
	ApplicationSpecification m_Specification;
	GLFWwindow* m_Window;
	std::vector<std::shared_ptr<Layer>> m_Layers;

	static Application* s_Instance;
	float m_DeltaTime;
};