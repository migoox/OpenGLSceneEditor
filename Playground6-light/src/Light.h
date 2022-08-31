#pragma once
#include "Cuboid.h"

class Light : public Transform
{
public:
	Light();
	virtual ~Light() { }

	void SetAmbient(glm::vec3 ambient)  { m_Ambient = ambient; }
	void SetDiffuse(glm::vec3 diffuse)  { m_Diffuse = diffuse; }
	void SetSpecular(glm::vec3 specular)  { m_Specular = specular; }

	glm::vec3 GetAmbient() const { return m_Ambient; }
	glm::vec3 GetDiffuse() const { return m_Diffuse; }
	glm::vec3 GetSpecular() const { return m_Specular; }

	glm::vec3 GetPosition() const { return this->GetTranslation(); }

	Cuboid& GetRepresentation() { return m_Cuboid; }

	// This matrix is required to be updated when drawing light source.
	// It is since we've got matrix of representation and of light, so we have to
	// combine these two in order to get matrix representing a model.
	glm::mat4 GetModelMatrix() const;

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

private:
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

	Cuboid m_Cuboid;

	static unsigned int m_NextIndex;
	unsigned int m_Index;
};


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

	virtual unsigned int GetIndex() override { return m_Index; }
private:
	static unsigned int m_NextIndex;
	unsigned int m_Index;
};

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

	virtual unsigned int GetIndex() override { return m_Index; }
private:
	float m_Constant;
	float m_Linear;
	float m_Quadratic;

	static unsigned int m_NextIndex;
	unsigned int m_Index;
};

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

	virtual unsigned int GetIndex() override { return m_Index; }

private:
	// in radians
	float m_CutOffAngle;
	float m_OuterCutOffAngle; 

	static unsigned int m_NextIndex;
	unsigned int m_Index;
};
