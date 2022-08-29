#include "Scene.h"
#include "Light.h"
#include "Model.h"
#include "Renderer.h"
#include "Input.h"
#include "Application.h"

// to do: shader manager
std::shared_ptr<Shader> ObjectShader;
std::shared_ptr<Shader> LightSourceShader;
// --------------------

void Scene::OnAttach()
{
	ObjectShader = std::shared_ptr<Shader>(new Shader("res/shaders/Material.shader"));
	LightSourceShader = std::shared_ptr<Shader>(new Shader("res/shaders/LightSource.shader"));
	m_PropetiesPanel.Init();
	m_NodeTreePanel.Init();
	m_SelectedIndex = -1;
	m_EPressed = false;

	m_LightsBoxes = false;
	m_SelectionVisibility = false;

	m_Selection.Create(0.4f);
	m_Selection.SetOrigin(glm::vec3(0.2f, 0.2f, 0.2f));
	m_Selection.SetColor(glm::vec4(0.f, 1.f, 1.f, 0.15f));

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

	// update lights
	for (auto& node : m_Nodes)
	{
		if (node.GetObjectType() == typeid(Light).hash_code() ||
			node.GetObjectType() == typeid(DirectionalLight).hash_code() ||
			node.GetObjectType() == typeid(PointLight).hash_code() ||
			node.GetObjectType() == typeid(Spotlight).hash_code())
		{
			auto light = static_cast<Light*>(node.GetObject().get());
			light->UpdateShader(*ObjectShader);

			LightSourceShader->Bind();
			LightSourceShader->SetUniformMat4f("u_ModelMatrix", light->GetModelMatrix());
			LightSourceShader->SetUniform4f("u_LightColor", light->GetRepresentation().GetColor());
			LightSourceShader->Unbind();

			if(m_LightsBoxes)
				Renderer::Draw(light->GetRepresentation().GetMesh(), *LightSourceShader);
		}
	}

	// update cuboids
	for (auto& node : m_Nodes)
	{
		if (node.GetObjectType() == typeid(Cube).hash_code())
		{
			auto cube = static_cast<Cube*>(node.GetObject().get());

			ObjectShader->Bind();
			ObjectShader->SetUniformMat4f("u_ModelMatrix", cube->GetModelMatrix());
			ObjectShader->SetUniformMat3f("u_NormalMatrix", cube->GetNormalMatrix());
			ObjectShader->Unbind();

			Renderer::Draw(cube->GetMesh(), *ObjectShader, cube->GetMaterial());
		}
	}

	if (m_SelectedIndex >= 0)
	{
		auto trans = m_Nodes[m_SelectedIndex].GetObjectTransform();
		m_Selection.SetTranslation(trans.GetTranslation());
		m_Selection.SetRotation(trans.GetEulers());
		LightSourceShader->Bind();
		LightSourceShader->SetUniformMat4f("u_ModelMatrix", m_Selection.GetModelMatrix());
		LightSourceShader->SetUniformMat3f("u_NormalMatrix", m_Selection.GetNormalMatrix());
		LightSourceShader->SetUniform4f("u_LightColor", m_Selection.GetColor());
		LightSourceShader->Unbind();
		
		if(m_SelectionVisibility)
			Renderer::Draw(m_Selection.GetMesh(), *LightSourceShader);
	}
	m_FPS = 1.f / dTime;
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
	ImGui::Checkbox("Show light boxes ", &m_LightsBoxes);
	ImGui::Checkbox("Selection visibility", &m_SelectionVisibility);
	ImGui::End();
}

void Scene::InitCubes(unsigned int count)
{
	srand(time(NULL));
	auto random = []() -> float { return (float)(rand() % 1999) / 1000.f - 1.f; };

	std::shared_ptr<Texture> texture1 = std::shared_ptr<Texture>(new Texture("res/textures/dice.png"));
	std::shared_ptr<Texture> specularMap1 = std::shared_ptr<Texture>(new Texture("res/specular_maps/marble_dice_specular.png"));
	std::shared_ptr<Texture> texture2 = std::shared_ptr<Texture>(new Texture("res/textures/box.png"));
	std::shared_ptr<Texture> specularMap2 = std::shared_ptr<Texture>(new Texture("res/specular_maps/box_specular.png"));

	Material mat1 = { {specularMap1}, {texture1}, 32.f };
	Material mat2 = { {specularMap2}, {texture2}, 256.f };

	for (unsigned int i = 0; i < count; i++)
	{
		Node node;
		node.Create<Cube>();
		node.SetName("Cube " + std::to_string(i));
		auto cube = static_cast<Cube*>(node.GetObject().get());
		cube->Create(0.3f);
		cube->SetMaterial(mat1);
		cube->SetOrigin(glm::vec3(0.15f, 0.15f, 0.15f));
		glm::vec3 dir = glm::normalize(glm::vec3(random(), random(), random()));
		float magnitude = 0.5f + ((random() + 1.f) / 2.f) * 1.f;
		cube->Translate(dir * magnitude);
		cube->SetRotation(glm::vec3(random() * glm::pi<float>(),
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
	dlight->SetRotation(glm::vec3(-0.5f, 0.f, 0.f));
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
	slight->SetRotation(glm::vec3(0.f, glm::radians(180.f), 0.f));
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
				m_Camera.Freeze(false);
				Input::SetCursorMode(CursorMode::Locked);
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
	ObjectShader->Bind();
	ObjectShader->SetUniformMat4f("u_ViewMatrix", m_Camera.GetViewMatrix());
	ObjectShader->SetUniformMat4f("u_ProjectionMatrix", m_Camera.GetProjectionMatrix());
	ObjectShader->SetUniform3f("u_ViewerPosition", m_Camera.GetTranslation());
	ObjectShader->Unbind();

	// update light source shader
	LightSourceShader->Bind();
	LightSourceShader->SetUniformMat4f("u_ViewMatrix", m_Camera.GetViewMatrix());
	LightSourceShader->SetUniformMat4f("u_ProjectionMatrix", m_Camera.GetProjectionMatrix());
	LightSourceShader->Unbind();
}
