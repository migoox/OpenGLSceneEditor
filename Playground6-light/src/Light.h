#pragma once
#include "Cuboid.h"

class Light : public Cuboid
{
public:
	Light() : 
		Cuboid(0.1f, 0.1f, 0.1f), 
		m_Ambient(glm::vec3(1.f)),
		m_Diffuse(glm::vec3(1.f)),
		m_Specular(glm::vec3(1.f)) { }

	~Light() { }

	void SetAmbient(glm::vec3 ambient)  { m_Ambient = ambient; }
	void SetDiffuse(glm::vec3 diffuse)  { m_Diffuse = diffuse; }
	void SetSpecular(glm::vec3 specular)  { m_Specular = specular; }

	void SetLightByColor(glm::vec3 color, glm::vec3 ambient = glm::vec3(0.5f), glm::vec3 diffuse = glm::vec3(0.2f))
	{
		m_Diffuse = color * diffuse;
		m_Ambient = m_Diffuse * ambient;
	}

	glm::vec3 GetAmbient() const { return m_Ambient; }
	glm::vec3 GetDiffuse() const { return m_Diffuse; }
	glm::vec3 GetSpecular() const { return m_Specular; }

	glm::vec3 GetPosition() const { return this->GetTranslation(); }

	virtual int GetType() const { return 0; }

	virtual void UpdateShader(Shader& shader) 
	{ 
		shader.Bind();
		shader.SetUniform1i("u_Light.type", this->GetType());
		shader.SetUniform3f("u_Light.position", this->GetTranslation());
		shader.SetUniform3f("u_Light.ambient", m_Ambient);
		shader.SetUniform3f("u_Light.diffuse", m_Diffuse);
		shader.SetUniform3f("u_Light.specular", m_Specular);
		shader.Unbind();
	}

private:
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight() : Light() { }
	virtual int GetType() const override { return 1; }
	glm::vec3 GetDirection() const { return this->GetForward(); }

	virtual void UpdateShader(Shader& shader) override
	{
		shader.Bind();
		shader.SetUniform1i("u_Light.type", this->GetType());
		shader.SetUniform3f("u_Light.position", this->GetTranslation());
		shader.SetUniform3f("u_Light.direction", this->GetDirection());
		shader.SetUniform3f("u_Light.ambient", this->GetAmbient());
		shader.SetUniform3f("u_Light.diffuse", this->GetDiffuse());
		shader.SetUniform3f("u_Light.specular", this->GetSpecular());
		shader.Unbind();
	}
};

class PointLight : public Light
{
public:
	PointLight() : Light(), m_Constant(1.f), m_Linear(1.f), m_Quadratic(1.f) { }
	PointLight(float constant, float linear, float quadratic) : 
		Light(), 
		m_Constant(constant),
		m_Linear(linear),
		m_Quadratic(quadratic)
	{ }

	virtual int GetType() const override { return 2; }
	
	void SetConstant(float constant) { m_Constant = constant; }
	float GetConstant() const { return m_Constant; }

	void SetLinear(float linear) { m_Linear = linear; }
	float GetLinear() const { return m_Linear; }

	void SetQuadratic(float quadratic) { m_Quadratic = quadratic; }
	float GetQuadratic() const { return m_Quadratic; }

	virtual void UpdateShader(Shader& shader) override
	{
		shader.Bind();
		shader.SetUniform1i("u_Light.type", this->GetType());
		shader.SetUniform3f("u_Light.position", this->GetTranslation());
		shader.SetUniform1f("u_Light.constant", m_Constant);
		shader.SetUniform1f("u_Light.linear", m_Linear);
		shader.SetUniform1f("u_Light.quadratic", m_Quadratic);
		shader.SetUniform3f("u_Light.diffuse", this->GetDiffuse());
		shader.SetUniform3f("u_Light.specular", this->GetSpecular());
		shader.Unbind();
	}

private:
	float m_Constant;
	float m_Linear;
	float m_Quadratic;
};

class Spotlight : public Light
{
public:
	Spotlight() : Light(), m_CutOffAngle(glm::radians(12.5f)), m_OuterCutOffAngle(glm::radians(17.5f)) { }

	virtual int GetType() const override { return 3; }
	glm::vec3 GetDirection() const { return this->GetForward(); }

	void SetCutOffAngle(float radians) { m_CutOffAngle = radians; }
	float GetCutOffAngle() const { return m_CutOffAngle; }

	void SetOuterCutOffAngle(float radians) { m_OuterCutOffAngle = radians; }
	float GetOuterCutOffAngle() const { return m_OuterCutOffAngle; }

	virtual void UpdateShader(Shader& shader) override
	{
		shader.Bind();
		shader.SetUniform1i("u_Light.type", this->GetType());
		shader.SetUniform3f("u_Light.position", this->GetTranslation());
		shader.SetUniform3f("u_Light.direction", this->GetDirection());
		shader.SetUniform1f("u_Light.cutOff", m_CutOffAngle);
		shader.SetUniform1f("u_Light.outerCutOff", m_OuterCutOffAngle);
		shader.SetUniform3f("u_Light.ambient", this->GetAmbient());
		shader.SetUniform3f("u_Light.diffuse", this->GetDiffuse());
		shader.SetUniform3f("u_Light.specular", this->GetSpecular());
		shader.Unbind();
	}

private:
	float m_CutOffAngle;
	float m_OuterCutOffAngle;
};