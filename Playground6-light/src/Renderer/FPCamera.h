#pragma once
#include "Camera.h"
#include "Input.h"
#include "glm/glm.hpp"

class FPCamera : public Camera
{
public:
	FPCamera() :
		m_LastPos(glm::vec2(0.f, 0.f)),
		m_Freezed(false),
		m_MovementSpeed(1.25f),
		m_Sensitivity(35.f)
	{ }

	void UpdateCamera(float dt);

	float GetMovementSpeed() { return m_MovementSpeed; }
	float GetSensitivity(float sensitivity) { return m_Sensitivity; }

	void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }
	void SetSensitivity(float sensitivity) { m_Sensitivity = sensitivity; }

	void Freeze(bool freeze) { m_Freezed = freeze; }
	bool IsFreezed() { return m_Freezed; }

private:
	float m_MovementSpeed;
	float m_Sensitivity;

	bool m_Freezed;
	glm::vec2 m_LastPos;
};

