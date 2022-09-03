#include "Scene.h"
#include "Light.h"
#include "Model.h"
#include "Renderer.h"
#include "Input.h"
#include "Application.h"
#include "ResourceManager.h"

void Scene::OnAttach()
{
	ResourceManager::AddShaderAlias("ObjectShader", "res/shaders/Material.shader");
	ResourceManager::AddShaderAlias("LightSourceShader", "res/shaders/LightSource.shader");
	m_PropetiesPanel.Init();
	m_NodeTreePanel.Init();
	m_SelectedIndex = -1;
	m_EPressed = false;

	m_LightBoxes = false;
	m_SelectionVisibility = false;

	Renderer::SetClearColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.f));
	Input::SetCursorMode(CursorMode::Locked);
	float width = (float)Application::GetSpecification().WindowSize.x;
	float height = (float)Application::GetSpecification().WindowSize.y;
	m_Camera.SetWidthByHeightRatio(width / height);

	InitCubes();
	InitLights();
}

void Scene::OnDetach()
{
}

void Scene::OnUpdate(float dTime)
{
	UpdateCamera(dTime);
	Renderer::ClearLights(*ResourceManager::GetShader("ObjectShader"));
	// update lights
	for (auto& node : m_Nodes)
	{
		if (!node.IsVisible()) continue;

		// draw representation with light source shader
		if (m_LightBoxes)
		{
			if (node.GetObjectType() == typeid(Light).hash_code() ||
				node.GetObjectType() == typeid(DirectionalLight).hash_code() ||
				node.GetObjectType() == typeid(PointLight).hash_code() ||
				node.GetObjectType() == typeid(Spotlight).hash_code())
			{
				auto light = static_cast<Light*>(node.GetObject().get());

				ResourceManager::GetShader("LightSourceShader")->Bind();
				ResourceManager::GetShader("LightSourceShader")->SetUniformMat4f("u_ModelMatrix", light->GetModelMatrix());
				ResourceManager::GetShader("LightSourceShader")->SetUniform4f("u_LightColor", light->GetRepresentation().GetColor());
				ResourceManager::GetShader("LightSourceShader")->Unbind();
				Renderer::Draw(light->GetRepresentation().GetMesh(), *ResourceManager::GetShader("LightSourceShader"));
			}
		}

		// update object shader 
		if (node.GetObjectType() == typeid(Light).hash_code())
		{
			auto light = static_cast<Light*>(node.GetObject().get());
			Renderer::PushLight(*light, *ResourceManager::GetShader("ObjectShader"));
		}
		if (node.GetObjectType() == typeid(DirectionalLight).hash_code())
		{
			auto light = static_cast<DirectionalLight*>(node.GetObject().get());
			Renderer::PushLight(*light, *ResourceManager::GetShader("ObjectShader"));
		}
		if (node.GetObjectType() == typeid(PointLight).hash_code())
		{
			auto light = static_cast<PointLight*>(node.GetObject().get());
			Renderer::PushLight(*light, *ResourceManager::GetShader("ObjectShader"));
		}
		if (node.GetObjectType() == typeid(Spotlight).hash_code())
		{
			auto light = static_cast<Spotlight*>(node.GetObject().get());
			Renderer::PushLight(*light, *ResourceManager::GetShader("ObjectShader"));
		}
	}

	// update models/cuboids
	for (auto& node : m_Nodes)
	{
		if (!node.IsVisible()) continue;

		ResourceManager::GetShader("ObjectShader")->Bind();
		ResourceManager::GetShader("ObjectShader")->SetUniformMat4f("u_ModelMatrix", node.GetObjectTransform().GetModelMatrix());
		ResourceManager::GetShader("ObjectShader")->SetUniformMat3f("u_NormalMatrix", node.GetObjectTransform().GetNormalMatrix());
		ResourceManager::GetShader("ObjectShader")->Unbind();

		if (node.GetObjectType() == typeid(Cube).hash_code())
		{
			auto cube = static_cast<Cube*>(node.GetObject().get());

			Renderer::Draw(cube->GetMesh(), *ResourceManager::GetShader("ObjectShader"), cube->GetMaterial());
		}
		else if (node.GetObjectType() == typeid(Model).hash_code())
		{
			auto model = static_cast<Model*>(node.GetObject().get());

			for(auto &mesh : model->GetMeshes())
				Renderer::Draw(mesh, *ResourceManager::GetShader("ObjectShader"), model->GetMaterial());
		}
	}

	if (m_SelectedIndex >= 0 && m_SelectionVisibility)
	{
		m_ItemSelection.OnUpdate(m_Nodes[m_SelectedIndex], *ResourceManager::GetShader("LightSourceShader"));
	}

	m_FPS = (int)(1.f / dTime);

	if (m_SelectedIndex >= 0 && !m_PropetiesPanel.IsFocused() && !m_NodeTreePanel.IsFocused())
		UpdateTransform(Application::GetWindowHandle(), &m_Nodes[m_SelectedIndex].GetObjectTransform(), dTime);
}

void Scene::OnImGuiRender()
{
	if (m_SelectedIndex >= 0)
		m_PropetiesPanel.Display(m_Nodes[m_SelectedIndex]);
	else
		m_PropetiesPanel.DisplayBlank();

	m_NodeTreePanel.Display(m_Nodes, m_SelectedIndex);

	ImGui::Begin("Editor propeties");
	std::string text = "FPS: " + std::to_string(m_FPS);
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 102, 255, 255));
	ImGui::Text(text.c_str());
	ImGui::PopStyleColor();
	ImGui::Checkbox("Show light boxes ", &m_LightBoxes);
	ImGui::Checkbox("Selection visibility", &m_SelectionVisibility);
	ImGui::End();
}

void Scene::InitCubes(unsigned int count)
{
	srand(time(NULL));
	auto random = []() -> float { return (float)(rand() % 1999) / 1000.f - 1.f; };

	Material mat1 = { 32.f, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
		{ ResourceManager::GetTexture("res/specular_maps/marble_dice_specular.png"),
		ResourceManager::GetTexture("res/specular_maps/box_specular.png") },

		{ ResourceManager::GetTexture("res/textures/dice.png"),
		ResourceManager::GetTexture("res/textures/box.png")} };
	for (unsigned int i = 0; i < count; i++)
	{
		Node node;
		node.Create<Cube>();
		node.SetName("Cube " + std::to_string(i));
		auto cube = static_cast<Cube*>(node.GetObject().get());
		cube->SetScale(glm::vec3(0.3f));
		cube->SetMaterial(mat1);
		cube->SetOrigin(glm::vec3(0.15f, 0.15f, 0.15f));
		glm::vec3 dir = glm::normalize(glm::vec3(random(), random(), random()));
		float magnitude = 0.5f + ((random() + 1.f) / 2.f) * 1.f;
		cube->Translate(dir * magnitude);

		cube->SetEulers(glm::vec3(random() * glm::pi<float>(),
				random() * glm::pi<float>(),
				random() * glm::pi<float>()));

		cube->Translate(glm::vec3(0.f, 0.f, 2.f));

		m_Nodes.push_back(node);
	}
}

void Scene::InitLights()
{
	Node node1;
	node1.Create<DirectionalLight>();
	node1.SetName("Directional light");
	m_Nodes.push_back(node1);

	auto dlight = static_cast<Cube*>(node1.GetObject().get());
	dlight->SetTranslation(glm::vec3(0.f, 3.f, 3.f));
	dlight->SetEulers(glm::vec3(-0.5f, 0.f, 0.f));
	dlight->Rotate(glm::vec3(0.f, 3.f, 0.f), Space::Global);

	Node node2;
	node2.Create<PointLight>();
	node2.SetName("Point light");
	m_Nodes.push_back(node2);

	auto plight = static_cast<PointLight*>(node1.GetObject().get());
	plight->SetTranslation(glm::vec3(0.f, 0.f, 2.f));

	Node node3;
	node3.SetName("Spotlight");
	node3.Create<Spotlight>();
	m_Nodes.push_back(node3);

	auto slight = static_cast<Spotlight*>(node1.GetObject().get());
	slight->SetTranslation(glm::vec3(0.f, 0.f, 5.f));
	slight->SetEulers(glm::vec3(0.f, glm::radians(180.f), 0.f));
}

void Scene::UpdateCamera(float dTime)
{
	//freeze camera 
	if (Input::IsKeyDown(KeyCode::E))
	{
		if (!m_EPressed)
		{
			if (m_Camera.IsFreezed())
			{
				if (!m_PropetiesPanel.IsFocused() && !m_NodeTreePanel.IsFocused())
				{
					m_Camera.Freeze(false);
					Input::SetCursorMode(CursorMode::Locked);
				}
			}
			else
			{
				m_Camera.Freeze(true);
				Input::SetCursorMode(CursorMode::Normal);
			}
		}
		m_EPressed = true;
	}
	else
	{
		m_EPressed = false;
	}

	// update camera
	m_Camera.UpdateCamera(dTime);

	// update object shader
	ResourceManager::GetShader("ObjectShader")->Bind();
	ResourceManager::GetShader("ObjectShader")->SetUniformMat4f("u_ViewMatrix", m_Camera.GetViewMatrix());
	ResourceManager::GetShader("ObjectShader")->SetUniformMat4f("u_ProjectionMatrix", m_Camera.GetProjectionMatrix());
	ResourceManager::GetShader("ObjectShader")->SetUniform3f("u_ViewerPosition", m_Camera.GetTranslation());
	ResourceManager::GetShader("ObjectShader")->Unbind();

	// update light source shader
	ResourceManager::GetShader("LightSourceShader")->Bind();
	ResourceManager::GetShader("LightSourceShader")->SetUniformMat4f("u_ViewMatrix", m_Camera.GetViewMatrix());
	ResourceManager::GetShader("LightSourceShader")->SetUniformMat4f("u_ProjectionMatrix", m_Camera.GetProjectionMatrix());
	ResourceManager::GetShader("LightSourceShader")->Unbind();
}

void Scene::UpdateTransform(GLFWwindow* window, Transform* transform, float dt)
{
	const int rotSpeed = 2 * glm::pi<float>() / 2.0f;
	const int moveSpeed = 2.5f;
	int state;
	float roll = 0.f, pitch = 0.f, yaw = 0.f;
	bool hasToRotate = false;
	state = glfwGetKey(window, GLFW_KEY_Z);
	if (state == GLFW_PRESS)
	{
		hasToRotate = true;
		pitch = rotSpeed * dt;
	}

	state = glfwGetKey(window, GLFW_KEY_X);
	if (state == GLFW_PRESS)
	{
		hasToRotate = true;
		yaw = rotSpeed * dt;
	}

	state = glfwGetKey(window, GLFW_KEY_C);
	if (state == GLFW_PRESS)
	{
		hasToRotate = true;
		roll = rotSpeed * dt;
	}
	state = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);

	if (hasToRotate)
	{
		if (state == GLFW_PRESS)
		{
			transform->Rotate(glm::vec3(pitch, yaw, roll), Space::Global);
		}
		else
		{
			transform->Rotate(glm::vec3(pitch, yaw, roll), Space::Local);
		}
	}

	state = glfwGetKey(window, GLFW_KEY_I);
	if (state == GLFW_PRESS)
	{
		transform->Translate(glm::vec3(0.00f, 0.0f, moveSpeed * dt), Space::Local);
	}

	state = glfwGetKey(window, GLFW_KEY_K);
	if (state == GLFW_PRESS)
	{
		transform->Translate(glm::vec3(0.00f, 0.0f, -moveSpeed * dt), Space::Local);
	}

	state = glfwGetKey(window, GLFW_KEY_J);
	if (state == GLFW_PRESS)
	{
		transform->Translate(glm::vec3(-moveSpeed * dt, 0.0f, 0.f), Space::Local);
	}

	state = glfwGetKey(window, GLFW_KEY_L);
	if (state == GLFW_PRESS)
	{
		transform->Translate(glm::vec3(moveSpeed * dt, 0.0f, 0.f), Space::Local);
	}

	state = glfwGetKey(window, GLFW_KEY_P);
	if (state == GLFW_PRESS)
	{
		transform->Translate(glm::vec3(0.0f, moveSpeed * dt, 0.0f), Space::Local);
	}

	state = glfwGetKey(window, GLFW_KEY_O);
	if (state == GLFW_PRESS)
	{
		transform->Translate(glm::vec3(0.0f, -moveSpeed * dt, 0.0f), Space::Local);
	}
}
