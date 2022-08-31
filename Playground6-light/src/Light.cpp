#include "Light.h"

unsigned int Light::m_NextIndex = 0;
unsigned int DirectionalLight::m_NextIndex = 0;
unsigned int PointLight::m_NextIndex = 0;
unsigned int Spotlight::m_NextIndex = 0;

Light::Light() :
	m_Ambient(glm::vec3(0.2f)),
	m_Diffuse(glm::vec3(0.7f)),
	m_Specular(glm::vec3(0.9f))
{
	m_Index = m_NextIndex++;
	m_Cuboid.SetScale(glm::vec3(0.1f));
	m_Cuboid.SetOrigin(glm::vec3(0.05f));
	m_Cuboid.SetColor(glm::vec4(0.7f, 0.7f, 0.7f, 1.f));
}

glm::mat4 Light::GetModelMatrix() const
{
	// In this case light matrix is a parent and cuboid matrix is a child.
	return Transform::GetModelMatrix() * m_Cuboid.GetModelMatrix();
}
