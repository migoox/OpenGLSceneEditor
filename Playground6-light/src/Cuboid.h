#pragma once
#include "Mesh.h"
#include "Transfrom.h"
#include "Material.h"
#include "Texture.h"
#include "SceneObject.h"
#include "glm/glm.hpp"

class Cuboid : public SceneObject
{
public:
	Cuboid(float width, float length, float height);
	~Cuboid();

	const Mesh& GetMesh() const { return m_Mesh; }
	
	void SetColor(glm::vec4 color) { m_Color = color; }
	glm::vec4 GetColor() const { return m_Color; }

	virtual void ImGuiDrawInfoPanel() override
	{
		std::string text;

		text = "Position" + glm::to_string(this->GetTranslation());
		ImGui::Text(text.c_str());

		text = "Rotation" + glm::to_string(this->GetEulers());
		ImGui::Text(text.c_str());

		text = "Scale" + glm::to_string(this->GetScale());
		ImGui::Text(text.c_str());
	}

	virtual std::string GetName() override { return "Cuboid"; }

private:
	Mesh m_Mesh;
	glm::vec4 m_Color;
};

class Cube : public Cuboid
{
public:
	Cube(float x) : Cuboid(x, x, x) { }
	~Cube() { }

	void SetSimpleMaterial(SimpleMaterial material) { m_SimpleMaterial = material; }
	SimpleMaterial GetSimpleMaterial() const { return m_SimpleMaterial; }

	void SetMaterial(Material material) { m_Material = material; }
	Material GetMaterial() const { return m_Material; }

private:
	SimpleMaterial m_SimpleMaterial;
	Material m_Material;
};