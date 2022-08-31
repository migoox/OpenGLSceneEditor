#include "Transfrom.h"

glm::vec3 Transform::GetEulersDegrees() const
{
	return glm::degrees(m_Eulers);
}

glm::vec3 Transform::GetEulers() const
{
	return m_Eulers;
}

glm::mat4 Transform::GetModelMatrix() const
{
	return  glm::translate(m_Translation) *
		glm::mat4_cast(m_Rotation) *
		glm::scale(m_Scale) *
		glm::translate(-m_Origin);
}

glm::mat3 Transform::GetNormalMatrix() const
{
	return glm::mat3(glm::mat4_cast(m_Rotation) *
		glm::scale(glm::vec3(1.f / m_Scale.x, 1.f / m_Scale.y, 1.f / m_Scale.z)));
}

void Transform::SetEulers(glm::vec3 eulers_rad)
{
	m_Rotation = ToQuaternion(eulers_rad);

	glm::mat4 rotMat = glm::mat4_cast(m_Rotation);
	m_Right = rotMat * glm::vec4(1.f, 0.f, 0.f, 1.f);
	m_Up = rotMat * glm::vec4(0.f, 1.f, 0.f, 1.f);
	m_Forward = rotMat * glm::vec4(0.f, 0.f, 1.f, 1.f);

	m_Eulers = eulers_rad;
}

void Transform::LookAt(glm::vec3 target, glm::vec3 up)
{
	// "-" appears because +z is forward axis and lookAt needs vector opposite to it
	// m_Up appears because of glitches with BaseUp
	m_Rotation = glm::quatLookAt(-glm::normalize(target - m_Translation), m_Up);

	glm::mat4 rotMat = glm::mat4_cast(m_Rotation);
	m_Right = rotMat * glm::vec4(1.f, 0.f, 0.f, 1.f);
	m_Up = rotMat * glm::vec4(0.f, 1.f, 0.f, 1.f);
	m_Forward = rotMat * glm::vec4(0.f, 0.f, 1.f, 1.f);

	m_Eulers = glm::eulerAngles(m_Rotation);
	if (m_Eulers.x < 0.f)
		m_Eulers.x += glm::pi<float>() * 2.f;
	if (m_Eulers.y < 0.f)
		m_Eulers.y += glm::pi<float>() * 2.f;
	if (m_Eulers.z < 0.f)
		m_Eulers.z += glm::pi<float>() * 2.f;
}

void Transform::Rotate(glm::vec3 eulers_rad, Space relativeTo)
{
	if (relativeTo == Space::Local)
	{
		m_Rotation = m_Rotation *
			glm::angleAxis(eulers_rad.x, glm::vec3(1.0f, 0.f, 0.f)) *	// yaw
			glm::angleAxis(eulers_rad.y, glm::vec3(0.0f, 1.f, 0.f)) * 	// pitch
			glm::angleAxis(eulers_rad.z, glm::vec3(0.0f, 0.f, 1.f));	// roll
	}
	else
	{
		m_Rotation =
			glm::angleAxis(eulers_rad.x, glm::vec3(1.0f, 0.f, 0.f)) *	// roll
			glm::angleAxis(eulers_rad.y, glm::vec3(0.0f, 1.f, 0.f)) *	// pitch
			glm::angleAxis(eulers_rad.z, glm::vec3(0.0f, 0.f, 1.f)) *	// yaw
			m_Rotation;
	}
	glm::mat4 rotMat = glm::mat4_cast(m_Rotation);
	m_Right = rotMat * glm::vec4(1.f, 0.f, 0.f, 1.f);
	m_Up = rotMat * glm::vec4(0.f, 1.f, 0.f, 1.f);
	m_Forward = rotMat * glm::vec4(0.f, 0.f, 1.f, 1.f);

	m_Eulers = glm::eulerAngles(m_Rotation);
	if (m_Eulers.x < 0.f)
		m_Eulers.x += glm::pi<float>() * 2.f;
	if (m_Eulers.y < 0.f)
		m_Eulers.y += glm::pi<float>() * 2.f;
	if (m_Eulers.z < 0.f)
		m_Eulers.z += glm::pi<float>() * 2.f;
}

void Transform::RotateAround(float euler_rad, glm::vec3 point, glm::vec3 axis)
{
	// TO DO
}

void Transform::Translate(glm::vec3 translation, Space relativeTo)
{
	if (relativeTo == Space::Local)
	{
		m_Translation += glm::mat3(m_Right,
			m_Up,
			m_Forward) * translation;
	}
	else
	{
		m_Translation += translation;
	}
}

glm::quat Transform::ToQuaternion(glm::vec3 eulers)
{
	double cy = std::cos(eulers.z * 0.5);
	double sy = std::sin(eulers.z * 0.5);
	double cp = std::cos(eulers.y * 0.5);
	double sp = std::sin(eulers.y * 0.5);
	double cr = std::cos(eulers.x * 0.5);
	double sr = std::sin(eulers.x * 0.5);

	glm::quat q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

