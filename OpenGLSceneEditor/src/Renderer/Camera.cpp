#include "Camera.h"

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::transpose(glm::mat4(
									glm::vec4(this->GetRight(),   0.f),
									glm::vec4(this->GetUp(),      0.f),
									glm::vec4(this->GetForward(), 0.f),
									glm::vec4(0.f, 0.f, 0.f,      1.f))) *
					      glm::translate(-this->GetTranslation());
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

glm::mat4 Camera::GetCameraMatrix() const
{
	return m_ProjectionMatrix * GetViewMatrix();
}

void Camera::SetFOVVertical(float fovy)
{
	m_FOVY = fovy;

	// reverting z axis is required to get right-handed orientation
	m_ProjectionMatrix = glm::perspective(m_FOVY, m_Ratio, m_ZNear, m_ZFar) * glm::scale(glm::vec3(1.f, 1.f, -1.f));
}

void Camera::SetWidthByHeightRatio(float ratio)
{
	m_Ratio = ratio;

	// reverting z axis is required to get right-handed orientation
	m_ProjectionMatrix = glm::perspective(m_FOVY, m_Ratio, m_ZNear, m_ZFar) * glm::scale(glm::vec3(1.f, 1.f, -1.f));
}

void Camera::SetZNear(float zNear)
{
	m_ZNear = zNear;

	// reverting z axis is required to get right-handed orientation
	m_ProjectionMatrix = glm::perspective(m_FOVY, m_Ratio, m_ZNear, m_ZFar) * glm::scale(glm::vec3(1.f, 1.f, -1.f));
}

void Camera::SetZFar(float zFar)
{
	m_ZFar = zFar;

	// reverting z axis is required to get right-handed orientation
	m_ProjectionMatrix = glm::perspective(m_FOVY, m_Ratio, m_ZNear, m_ZFar) * glm::scale(glm::vec3(1.f, 1.f, -1.f));
}

