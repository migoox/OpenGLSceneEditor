#pragma once
#include "Cube.h"

class Light : public Transform
{
public:
	Light() :
		m_Ambient(glm::vec3(0.2f)),
		m_Diffuse(glm::vec3(0.7f)),
		m_Specular(glm::vec3(0.9f))
	{
		this->SetScale(glm::vec3(0.1f));
		this->SetOrigin(glm::vec3(0.5f));
		m_Cube.SetColor(glm::vec4(0.7f, 0.7f, 0.7f, 1.f));
	}

	virtual ~Light() { }

	void SetAmbient(glm::vec3 ambient)  { m_Ambient = ambient; }
	void SetDiffuse(glm::vec3 diffuse)  { m_Diffuse = diffuse; }
	void SetSpecular(glm::vec3 specular)  { m_Specular = specular; }

	glm::vec3 GetAmbient() const { return m_Ambient; }
	glm::vec3 GetDiffuse() const { return m_Diffuse; }
	glm::vec3 GetSpecular() const { return m_Specular; }

	glm::vec3 GetPosition() const { return this->GetTranslation(); }

	Cube& GetRepresentation() { return m_Cube; }

	// This matrix is required to be updated when drawing light source.
	// It is since we've got matrix of representation and of light, so we have to
	// combine these two in order to get matrix representing a model.
	glm::mat4 GetModelMatrix() const
	{
		// In this case light matrix is a parent and cuboid matrix is a child.
		return Transform::GetModelMatrix() * m_Cube.GetModelMatrix();
	}

	static unsigned int GetDefaultLightMaxInstancesCount()		{ return 10; }
	static unsigned int GetDirectionalLightMaxInstancesCount()	{ return 10; }
	static unsigned int GetPointLightMaxInstancesCount()		{ return 10; }
	static unsigned int GetSpotlightMaxInstancesCount()			{ return 10; }

private:
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

	Cube m_Cube;
};


class DirectionalLight : public Light
{
public:
	DirectionalLight() : Light() 
	{ }
	glm::vec3 GetDirection() const { return this->GetForward(); }

private:
	static unsigned int m_NextIndex;
	unsigned int m_Index;
};

class PointLight : public Light
{
public:
	PointLight() : Light(), m_Constant(1.f), m_Linear(1.f), m_Quadratic(1.f) 
	{ }

	PointLight(float constant, float linear, float quadratic) : 
		Light(), 
		m_Constant(constant),
		m_Linear(linear),
		m_Quadratic(quadratic)
	{ }

	
	void SetConstant(float constant) { m_Constant = constant; }
	float GetConstant() const { return m_Constant; }

	void SetLinear(float linear) { m_Linear = linear; }
	float GetLinear() const { return m_Linear; }

	void SetQuadratic(float quadratic) { m_Quadratic = quadratic; }
	float GetQuadratic() const { return m_Quadratic; }

private:
	float m_Constant;
	float m_Linear;
	float m_Quadratic;
};

class Spotlight : public Light
{
public:
	Spotlight() : Light(), m_CutOffAngle(glm::radians(12.5f)), m_OuterCutOffAngle(glm::radians(17.5f)) 
	{ }

	glm::vec3 GetDirection() const { return this->GetForward(); }

	void SetCutOffAngle(float radians) { m_CutOffAngle = radians; }
	float GetCutOffAngle() const { return m_CutOffAngle; }

	void SetOuterCutOffAngle(float radians) { m_OuterCutOffAngle = radians; }
	float GetOuterCutOffAngle() const { return m_OuterCutOffAngle; }

private:
	// in radians
	float m_CutOffAngle;
	float m_OuterCutOffAngle; 
};
