#include <glm/gtc/type_ptr.hpp>
#include "PropetiesPanel.h"
#include "Light.h"
#include "Model.h"
#include "ResourceManager.h"

void UIPropetiesPanel::Init()
{
	m_LightSection.Init();
	m_ModelSection.Init();
	m_MaterialSection.Init();
}

void UIPropetiesPanel::DisplayBlank()
{
	ImGui::Begin("Propeties panel");
	ImGui::Text("Nothing has been selected.");
	m_Focused = ImGui::IsWindowFocused();
	ImGui::End();
}

void UIPropetiesPanel::Display(Node& node)
{
	ImGui::Begin("Propeties panel");
	static std::string name = "";
	{
		name = node.GetName();
		ImGui::InputText("Name ", &name);
		node.SetName(name);
	}
	static bool visibility = true;
	{
		visibility = node.IsVisible();
		ImGui::Checkbox("Visibility", &visibility);
		node.SetVisibility(visibility);
	}
	m_TransformSection.Display(node);
	m_LightSection.Display(node);

	if (node.GetObjectType() == typeid(Cube).hash_code())
	{
		auto cube = static_cast<Cube*>(node.GetObject().get());

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(180, 0, 230, 255));
		ImGui::Text("Cube");
		ImGui::PopStyleColor();

		ImGui::Separator();
	}
	else if (node.GetObjectType() == typeid(Model).hash_code())
	{
		m_ModelSection.Display(node);
	}

	m_MaterialSection.Display(node);

	m_Focused = ImGui::IsWindowFocused();
	ImGui::End();
}

void UIPropetiesPanelTransformSection::Display(Node& node)
{
	auto& trans = node.GetObjectTransform();
	auto pos = trans.GetTranslation();
	auto rot = trans.GetEulersDegrees();
	auto scal = trans.GetScale();

	m_Position[0] = pos.x;	m_Position[1] = pos.y;	m_Position[2] = pos.z;
	m_Eulers[0] = rot.x;	m_Eulers[1] = rot.y;	m_Eulers[2] = rot.z;
	m_Scale[0] = scal.x;	m_Scale[1] = scal.y;	m_Scale[2] = scal.z;


	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(180, 0, 230, 255));
	ImGui::Text("Transform");
	ImGui::PopStyleColor();

	ImGui::Separator();
	ImGui::Text("Position");

		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0, 0, 100));
		ImGui::InputFloat("X", m_Position);
		ImGui::PopStyleColor();


		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 255, 0, 100));
		ImGui::InputFloat("Y", &m_Position[1]);
		ImGui::PopStyleColor();


		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 255, 100));
		ImGui::InputFloat("Z", &m_Position[2]);
		ImGui::PopStyleColor();

	ImGui::Separator();
	ImGui::Text("Eulers (ZYX)");

		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0, 0, 100));
		ImGui::InputFloat("pitch (X)", m_Eulers);
		ImGui::PopStyleColor();


		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 255, 0, 100));
		ImGui::InputFloat("yaw   (Y)", &m_Eulers[1]);
		ImGui::PopStyleColor();


		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 255, 100));
		ImGui::InputFloat("roll  (Z)", &m_Eulers[2]);
		ImGui::PopStyleColor();

	ImGui::Separator();
	ImGui::Text("Scale");

		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0, 0, 100));
		ImGui::InputFloat("x factor", m_Scale);
		ImGui::PopStyleColor();


		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 255, 0, 100));
		ImGui::InputFloat("y factor", &m_Scale[1]);
		ImGui::PopStyleColor();


		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 255, 100));
		ImGui::InputFloat("z factor", &m_Scale[2]);
		ImGui::PopStyleColor();

	ImGui::Separator();

	trans.SetTranslation(glm::vec3(m_Position[0], m_Position[1], m_Position[2]));
	trans.SetEulers(glm::vec3(glm::radians(m_Eulers[0]), glm::radians(m_Eulers[1]), glm::radians(m_Eulers[2])));
	trans.SetScale(glm::vec3(m_Scale[0], m_Scale[1], m_Scale[2]));
}

void UIPropetiesPanelLightSection::Init()
{
	m_LightHash = typeid(Light).hash_code();
	m_DirLightHash = typeid(DirectionalLight).hash_code();
	m_PointLightHash = typeid(PointLight).hash_code();
	m_SpotlightHash = typeid(Spotlight).hash_code();
}

void UIPropetiesPanelLightSection::Display(Node& node)
{
	if (node.GetObjectType() == m_LightHash ||
		node.GetObjectType() == m_DirLightHash ||
		node.GetObjectType() == m_PointLightHash ||
		node.GetObjectType() == m_SpotlightHash)
	{
		auto light = static_cast<Light*>(node.GetObject().get());
		auto diff = light->GetDiffuse();
		auto amb = light->GetAmbient();

		m_Diffuse[0] = diff.x;	m_Diffuse[1] = diff.y;	m_Diffuse[2] = diff.z;
		m_Ambient[0] = amb.x;	m_Ambient[1] = amb.y;	m_Ambient[2] = amb.z;
		m_Specular = light->GetSpecular().x;

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(180, 0, 230, 255));
		ImGui::Text("Light");
		ImGui::PopStyleColor();

		ImGui::Separator();
		ImGui::ColorEdit3("Color", m_Diffuse);
		ImGui::ColorEdit3("Ambient", m_Ambient);
		ImGui::SliderFloat("Specular", &m_Specular, 0.f, 1.f);

		light->SetDiffuse(glm::vec3(m_Diffuse[0], m_Diffuse[1], m_Diffuse[2]));
		light->SetAmbient(glm::vec3(m_Ambient[0], m_Ambient[1], m_Ambient[2]));
		light->SetSpecular(glm::vec3(m_Specular));
		light->GetRepresentation().SetColor(glm::vec4(m_Diffuse[0], m_Diffuse[1], m_Diffuse[2], 1.f));
	}

	if (node.GetObjectType() == m_LightHash)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 102, 255, 255));
		ImGui::Text("Type: Default light");
		ImGui::PopStyleColor();

		ImGui::Separator();
	}

	if (node.GetObjectType() == m_DirLightHash)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 102, 255, 255));
		ImGui::Text("Type: Directional light");
		ImGui::PopStyleColor();

		ImGui::Separator();
	}

	if (node.GetObjectType() == m_PointLightHash)
	{
		auto light = static_cast<PointLight*>(node.GetObject().get());

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 102, 255, 255));
		ImGui::Text("Type: Point light");
		ImGui::PopStyleColor();

		m_Quadratic = light->GetQuadratic();
		m_Linear = light->GetLinear();

		ImGui::SliderFloat("Linear", &m_Linear, 0.f, 1.f);
		ImGui::SliderFloat("Quadratic", &m_Quadratic, 0.f, 1.f);

		light->SetQuadratic(m_Quadratic);
		light->SetLinear(m_Linear);

		ImGui::Separator();
	}

	if (node.GetObjectType() == m_SpotlightHash)
	{
		auto light = static_cast<Spotlight*>(node.GetObject().get());

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 102, 255, 255));
		ImGui::Text("Type: Spotlight");
		ImGui::PopStyleColor();

		m_CutOffAngle = glm::degrees(light->GetCutOffAngle());
		m_OuterCutOffAngle = glm::degrees(light->GetOuterCutOffAngle());

		ImGui::SliderFloat("Cut off", &m_CutOffAngle, 0.f, 160.f);
		ImGui::SliderFloat("Outer cut off", &m_OuterCutOffAngle, 0.f, 160.f);

		light->SetCutOffAngle(glm::radians(m_CutOffAngle));
		light->SetOuterCutOffAngle(glm::radians(m_OuterCutOffAngle));

		ImGui::Separator();
	}
}

void UIPropetiesPanelModelSection::Init()
{
	m_ModelHash = typeid(Model).hash_code();
}

void UIPropetiesPanelModelSection::Display(Node& node)
{
	if (node.GetObjectType() != m_ModelHash)
		return;

	auto model = static_cast<Model*>(node.GetObject().get());

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(180, 0, 230, 255));
	ImGui::Text("Model");
	ImGui::PopStyleColor();

	ImGui::Separator();

	// model loading
	ImGui::Text("Model path(.obj): ");
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));

	if (model->LoadingSucceded())
	{
		ImGui::Text(model->GetFilePath().c_str());
	}
	else
	{
		ImGui::Text("Nothing has been loaded.");
	}
	ImGui::PopStyleColor();

	ImGui::Text("Load Model: ");

	static std::string text = "";
	{
		ImGui::InputText("##model", &text);
	}

	ImGui::SameLine();
	static bool loaded = false;
	static bool error = false;
	if (ImGui::Button("Load##model"))
	{
		Model m;
		std::replace(text.begin(), text.end(), '\\', '/');
		m.LoadModel(text);
		if (m.LoadingSucceded())
		{
			*model = m;
			loaded = true;
			error = false;
			text = "";
		}
		else
		{
			loaded = false;
			error = true;
		}
	}

	if (!loaded && !error)
		ImGui::Text(" ");
	else if (loaded)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text("Model has been loaded.");
		ImGui::PopStyleColor();
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::Text("Couldn't load model.");
		ImGui::PopStyleColor();

	}
}

void UIPropetiesPanelMaterialSection::Init()
{
}

void UIPropetiesPanelMaterialSection::Display(Node& node)
{
	if (!node.HasMaterial())
		return;

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 102, 255, 255));
	ImGui::Text("Component: Material");
	ImGui::PopStyleColor();


	Material mat = node.GetMaterial();
	ImGui::ColorEdit3("Ambient",	glm::value_ptr(mat.Ambient));
	ImGui::ColorEdit3("Diffues",	glm::value_ptr(mat.Diffuse));
	ImGui::ColorEdit3("Specular",	glm::value_ptr(mat.Specular));
	
	std::string label;

	// texture
	ImGui::Text("Texture path: ");

	static unsigned int selectedIndex = 0;
	{
		ImGui::BeginChild("tree##texture", ImVec2(0, 64), true);
		for (int i = 0; i < mat.Texture.size(); i++)
		{
			label = "[" + std::to_string(i) + "] " + mat.Texture[i]->GetFilePath();
			if (ImGui::Selectable(label.c_str(), selectedIndex == i))
				selectedIndex = i;
		}
		ImGui::EndChild();
	}

	if (ImGui::Button("Remove##texture"))
	{
		if (!mat.Texture.empty())
			mat.Texture.erase(mat.Texture.begin() + selectedIndex);
	}
	ImGui::SameLine();
	if (ImGui::Button("v##texture"))
	{
		if (!mat.Texture.empty())
		{
			if(selectedIndex < mat.Texture.size() - 1)
				std::swap(mat.Texture[selectedIndex + 1], mat.Texture[selectedIndex]);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("^##texture"))
	{
		if (!mat.Texture.empty())
		{
			if (selectedIndex > 0)
				std::swap(mat.Texture[selectedIndex - 1], mat.Texture[selectedIndex]);
		}
	}
	ImGui::Text("Load Texture: ");

	static std::string text1 = "";
	{
		ImGui::InputText("##texture", &text1);
	}
	ImGui::SameLine();
	static bool loaded = false;
	static bool error = false;
	if (ImGui::Button("Load##texture"))
	{
		std::replace(text1.begin(), text1.end(), '\\', '/');
		auto texture = ResourceManager::GetTexture(text1);
		if (texture->IsLoadingSucceded())
		{
			mat.Texture.push_back(texture);
			loaded = true;
			error = false;
			text1 = "";
		}
		else
		{
			loaded = false;
			error = true;
		}
	}
	if(!loaded && !error)
		ImGui::Text(" ");
	else if (loaded)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text("Texure has been loaded.");
		ImGui::PopStyleColor();
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::Text("Couldn't load texture.");
		ImGui::PopStyleColor();

	}

	// specular map
	ImGui::Text("Specular map path: ");

	static unsigned int selectedIndex2 = 0;
	{
		ImGui::BeginChild("tree##specular", ImVec2(0, 64), true);
		for (int i = 0; i < mat.SpecularMap.size(); i++)
		{
			label = "[" + std::to_string(i) + "] " + mat.SpecularMap[i]->GetFilePath();
			if (ImGui::Selectable(label.c_str(), selectedIndex2 == i))
				selectedIndex2 = i;
		}
		ImGui::EndChild();
	}

	if (ImGui::Button("Remove##specular"))
	{
		if (!mat.SpecularMap.empty())
			mat.SpecularMap.erase(mat.SpecularMap.begin() + selectedIndex2);
	}
	ImGui::SameLine();
	if (ImGui::Button("v##specular"))
	{
		if (!mat.SpecularMap.empty())
		{
			if (selectedIndex2 < mat.SpecularMap.size() - 1)
				std::swap(mat.SpecularMap[selectedIndex2 + 1], mat.SpecularMap[selectedIndex2]);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("^##specular"))
	{
		if (!mat.SpecularMap.empty())
		{
			if (selectedIndex2 > 0)
				std::swap(mat.SpecularMap[selectedIndex2 - 1], mat.SpecularMap[selectedIndex2]);
		}
	}
	ImGui::Text("Load Specular map: ");

	static std::string text2 = "";
	{
		ImGui::InputText("##specular", &text2);
	}
	ImGui::SameLine();
	static bool loaded2 = false;
	static bool error2 = false;
	if (ImGui::Button("Load##specular"))
	{
		std::replace(text2.begin(), text2.end(), '\\', '/');
		auto speuclarMap = ResourceManager::GetTexture(text2);
		if (speuclarMap->IsLoadingSucceded())
		{
			mat.SpecularMap.push_back(speuclarMap);
			loaded2 = true;
			error2 = false;
			text2 = "";
		}
		else
		{
			loaded2 = false;
			error2 = true;
		}
	}

	if (!loaded2 && !error2)
		ImGui::Text(" ");
	else if (loaded2)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text("Specular map has been loaded.");
		ImGui::PopStyleColor();
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::Text("Couldn't load specular map.");
		ImGui::PopStyleColor();

	}
	/*ImGui::Text("Specular map path: ");

	for (auto& map : mat.SpecularMap)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
		if (mat.SpecularMap.size() > 0)
			ImGui::Text(mat.SpecularMap[0]->GetFilePath().c_str());
		else
			ImGui::Text("no specular map has been loaded.");
		ImGui::PopStyleColor();
	}
	{
		if (ImGui::Button("Load##map"))
		{

		}

		ImGui::SameLine();

		if (ImGui::Button("Remove##map"))
		{
			if (mat.Texture.size() > 0)
				mat.SpecularMap.pop_back();
		}
	}*/
	node.SetMaterial(mat);
}
