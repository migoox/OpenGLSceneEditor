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

	inline void SetAmbient(glm::vec3 ambient)  { m_Ambient = ambient; }
	inline void SetDiffuse(glm::vec3 diffuse)  { m_Diffuse = diffuse; }
	inline void SetSpecular(glm::vec3 specular)  { m_Specular = specular; }

	void SetLightByColor(glm::vec3 color, glm::vec3 ambient = glm::vec3(0.5f), glm::vec3 diffuse = glm::vec3(0.2f))
	{
		m_Diffuse = color * diffuse;
		m_Ambient = m_Diffuse * ambient;
	}

	inline glm::vec3 GetAmbient() const { return m_Ambient; }
	inline glm::vec3 GetDiffuse() const { return m_Diffuse; }
	inline glm::vec3 GetSpecular() const { return m_Specular; }

private:
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
};