#pragma once
#include "Cuboid.h"
#include "SceneObject.h"

class Light : public SceneObject
{
public:
	Light() : 
		m_Cuboid(Cuboid(0.1f, 0.1f, 0.1f)), 
		m_Ambient(glm::vec3(0.f)),
		m_Diffuse(glm::vec3(0.f)),
		m_Specular(glm::vec3(0.f)) 
	{
		this->SetOrigin(glm::vec3(0.05f));
		m_Index = m_NextIndex++;
		m_Cuboid.SetColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
	}

	~Light() { }

	void SetAmbient(glm::vec3 ambient)  { m_Ambient = ambient; }
	void SetDiffuse(glm::vec3 diffuse)  { m_Diffuse = diffuse; }
	void SetSpecular(glm::vec3 specular)  { m_Specular = specular; }

	void SetLightByColor(glm::vec3 color, glm::vec3 ambient = glm::vec3(0.5f), glm::vec3 diffuse = glm::vec3(0.2f))
	{
		m_Diffuse = color * diffuse;
		m_Ambient = m_Diffuse * ambient;
		m_Specular = glm::vec3((color.r + color.g + color.b) / 3.f);
	}

	glm::vec3 GetAmbient() const { return m_Ambient; }
	glm::vec3 GetDiffuse() const { return m_Diffuse; }
	glm::vec3 GetSpecular() const { return m_Specular; }

	glm::vec3 GetPosition() const { return this->GetTranslation(); }

	Cuboid& GetRepresentation() { return m_Cuboid; }

	virtual void UpdateShader(Shader& shader) 
	{ 
		shader.Bind();
		shader.SetUniform3f("u_Lights[" + std::to_string(m_Index) + "].position", this->GetTranslation());
		shader.SetUniform3f("u_Lights[" + std::to_string(m_Index) + "].ambient", m_Ambient);
		shader.SetUniform3f("u_Lights[" + std::to_string(m_Index) + "].diffuse", m_Diffuse);
		shader.SetUniform3f("u_Lights[" + std::to_string(m_Index) + "].specular", m_Specular);
		shader.SetUniform1i("u_DefaultLightsCount", m_NextIndex);
		shader.Unbind();
	}

	virtual unsigned int GetIndex() { return m_Index; }

	virtual void ImGuiDrawInfoPanel() override
	{
		//ImGui::SliderFloat3("Ambient", (float*)&m_Ambient, 0.f, 1.f);
		//ImGui::SliderFloat3("Diffuse", (float*)&m_Diffuse, 0.f, 1.f);
		//ImGui::SliderFloat3("Specular", (float*)&m_Specular, 0.f, 1.f);

		ImGui::ColorEdit3("Color", m_ImGuiColor);
		ImGui::SliderFloat2("Ambient, Diffuse", m_ImGuiAmbientDiff, 0.f, 1.f);

		this->SetLightByColor(glm::vec3(m_ImGuiColor[0], m_ImGuiColor[1], m_ImGuiColor[2]),
			glm::vec3(m_ImGuiAmbientDiff[0]), glm::vec3(m_ImGuiAmbientDiff[1]));
		m_Cuboid.SetColor(glm::vec4(m_ImGuiColor[0], m_ImGuiColor[1], m_ImGuiColor[2], 1.f));
	}

	virtual std::string GetName() override { return "Light"; }

private:
	float m_ImGuiColor[3];
	float m_ImGuiAmbientDiff[2];

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

	Cuboid m_Cuboid;

	static unsigned int m_NextIndex;
	unsigned int m_Index;
};
unsigned int Light::m_NextIndex = 0;

class DirectionalLight : public Light
{
public:
	DirectionalLight() : Light() 
	{ 
		m_Index = m_NextIndex++;
	}
	glm::vec3 GetDirection() const { return this->GetForward(); }

	virtual void UpdateShader(Shader& shader) override
	{
		shader.Bind();
		shader.SetUniform3f("u_DirectionalLights[" + std::to_string(m_Index) + "].direction", this->GetDirection());
		shader.SetUniform3f("u_DirectionalLights[" + std::to_string(m_Index) + "].ambient", this->GetAmbient());
		shader.SetUniform3f("u_DirectionalLights[" + std::to_string(m_Index) + "].diffuse", this->GetDiffuse());
		shader.SetUniform3f("u_DirectionalLights[" + std::to_string(m_Index) + "].specular", this->GetSpecular());
		shader.SetUniform1i("u_DirectionalLightsCount", m_NextIndex);
		shader.Unbind();
	}

	virtual void ImGuiDrawInfoPanel() override
	{
		Light::ImGuiDrawInfoPanel();
	}

	virtual std::string GetName() override { return "Directional Light"; }

	virtual unsigned int GetIndex() override { return m_Index; }
private:
	static unsigned int m_NextIndex;
	unsigned int m_Index;
};
unsigned int DirectionalLight::m_NextIndex = 0;

class PointLight : public Light
{
public:
	PointLight() : Light(), m_Constant(1.f), m_Linear(1.f), m_Quadratic(1.f) 
	{ 
		m_Index = m_NextIndex++;
	}
	PointLight(float constant, float linear, float quadratic) : 
		Light(), 
		m_Constant(constant),
		m_Linear(linear),
		m_Quadratic(quadratic)
	{ 
		m_Index = m_NextIndex++;
	}

	
	void SetConstant(float constant) { m_Constant = constant; }
	float GetConstant() const { return m_Constant; }

	void SetLinear(float linear) { m_Linear = linear; }
	float GetLinear() const { return m_Linear; }

	void SetQuadratic(float quadratic) { m_Quadratic = quadratic; }
	float GetQuadratic() const { return m_Quadratic; }

	virtual void UpdateShader(Shader& shader) override
	{
		shader.Bind();
		shader.SetUniform3f("u_PointLights[" + std::to_string(m_Index) + "].position", this->GetTranslation());
		shader.SetUniform3f("u_PointLights[" + std::to_string(m_Index) + "].ambient", this->GetAmbient());
		shader.SetUniform3f("u_PointLights[" + std::to_string(m_Index) + "].diffuse", this->GetDiffuse());
		shader.SetUniform3f("u_PointLights[" + std::to_string(m_Index) + "].specular", this->GetSpecular());
		shader.SetUniform1f("u_PointLights[" + std::to_string(m_Index) + "].constant", m_Constant);
		shader.SetUniform1f("u_PointLights[" + std::to_string(m_Index) + "].linear", m_Linear);
		shader.SetUniform1f("u_PointLights[" + std::to_string(m_Index) + "].quadratic", m_Quadratic);
		shader.SetUniform1i("u_PointLightsCount", m_NextIndex);
		shader.Unbind();
	}

	virtual void ImGuiDrawInfoPanel() override
	{
		Light::ImGuiDrawInfoPanel();
		//ImGui::SliderFloat("Constant", (float*)&m_Constant, 0.f, 1.f);
		ImGui::SliderFloat("Linear", (float*)&m_Linear, 0.f, 1.f);
		ImGui::SliderFloat("Quadratic", (float*)&m_Quadratic, 0.f, 1.f);
	}

	virtual std::string GetName() override { return "Point Light"; }

	virtual unsigned int GetIndex() override { return m_Index; }
private:
	float m_Constant;
	float m_Linear;
	float m_Quadratic;

	static unsigned int m_NextIndex;
	unsigned int m_Index;
};
unsigned int PointLight::m_NextIndex = 0;

class Spotlight : public Light
{
public:
	Spotlight() : Light(), m_CutOffAngle(glm::radians(12.5f)), m_OuterCutOffAngle(glm::radians(17.5f)) 
	{ 
		m_Index = m_NextIndex++;
	}

	glm::vec3 GetDirection() const { return this->GetForward(); }

	void SetCutOffAngle(float radians) { m_CutOffAngle = radians; }
	float GetCutOffAngle() const { return m_CutOffAngle; }

	void SetOuterCutOffAngle(float radians) { m_OuterCutOffAngle = radians; }
	float GetOuterCutOffAngle() const { return m_OuterCutOffAngle; }

	virtual void UpdateShader(Shader& shader) override
	{
		shader.Bind();
		shader.SetUniform3f("u_Spotlights[" + std::to_string(m_Index) + "].position", this->GetTranslation());
		shader.SetUniform3f("u_Spotlights[" + std::to_string(m_Index) + "].direction", this->GetDirection());
		shader.SetUniform3f("u_Spotlights[" + std::to_string(m_Index) + "].ambient", this->GetAmbient());
		shader.SetUniform3f("u_Spotlights[" + std::to_string(m_Index) + "].diffuse", this->GetDiffuse());
		shader.SetUniform3f("u_Spotlights[" + std::to_string(m_Index) + "].specular", this->GetSpecular());
		shader.SetUniform1f("u_Spotlights[" + std::to_string(m_Index) + "].cutOff", m_CutOffAngle);
		shader.SetUniform1f("u_Spotlights[" + std::to_string(m_Index) + "].outerCutOff", m_OuterCutOffAngle);
		shader.SetUniform1i("u_SpotlightsCount", m_NextIndex);
		shader.Unbind();
	}

	virtual void ImGuiDrawInfoPanel() override
	{
		Light::ImGuiDrawInfoPanel();
		//ImGui::SliderFloat("Constant", (float*)&m_Constant, 0.f, 1.f);
		ImGui::SliderFloat("CutOffAngle", (float*)&m_CutOffAngle, 0.f, glm::radians(100.f));
		ImGui::SliderFloat("OuterCutOffAngle", (float*)&m_OuterCutOffAngle, 0.f, glm::radians(170.f));
	}

	virtual std::string GetName() override { return "Spotlight"; }

	virtual unsigned int GetIndex() override { return m_Index; }
private:
	float m_CutOffAngle;
	float m_OuterCutOffAngle; 

	static unsigned int m_NextIndex;
	unsigned int m_Index;
};
unsigned int Spotlight::m_NextIndex = 0;
