#include "FPCamera.h"

void FPCamera::UpdateCamera(float dt)
{
	glm::vec2 mouse = Input::GetWindowMousePosition();

	float offsetX = glm::radians(-m_Sensitivity * (m_LastPos.x - mouse.x));
	float offsetY = glm::radians(m_Sensitivity * (m_LastPos.y - mouse.y));

	m_LastPos = mouse;

	if (m_Freezed)
	{
		return;
	}

	this->Rotate(glm::vec3(0.f, offsetX, 0.f), Space::Global);
	this->Rotate(glm::vec3(offsetY, 0.f, 0.f), Space::Local);

	auto front = this->GetForward();
	auto right = this->GetRight();

	front.y = 0.f;
	front = glm::normalize(front);

	right.y = 0.f;
	right = glm::normalize(right);

	if (Input::IsKeyDown(KeyCode::W))
		this->Translate(front * m_MovementSpeed * dt, Space::Global);
	if (Input::IsKeyDown(KeyCode::S))
		this->Translate(-front * m_MovementSpeed * dt, Space::Global);
	if (Input::IsKeyDown(KeyCode::D))
		this->Translate(right * m_MovementSpeed * dt, Space::Global);
	if (Input::IsKeyDown(KeyCode::A))
		this->Translate(-right * m_MovementSpeed * dt, Space::Global);
	if (Input::IsKeyDown(KeyCode::Space))
		this->Translate(glm::vec3(0.f, m_MovementSpeed * dt, 0.f), Space::Global);
	if (Input::IsKeyDown(KeyCode::LeftShift))
		this->Translate(glm::vec3(0.f, -m_MovementSpeed * dt, 0.f), Space::Global);
}
