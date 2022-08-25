#include "Input.h"
#include "Application.h"

bool Input::IsKeyDown(KeyCode keycode)
{
	int state = glfwGetKey(Application::GetWindowHandle(), (int)keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonDown(MouseButton button)
{
	int state = glfwGetMouseButton(Application::GetWindowHandle(), (int)button);
	return state == GLFW_PRESS;
}

glm::vec2 Input::GetWindowMousePosition()
{
	double xpos, ypos;
	glfwGetCursorPos(Application::GetWindowHandle(), &xpos, &ypos);
	auto win = Application::GetSpecification().WindowSize;
	float mouseX = (xpos - win.x / 2.f) / win.x * 2.0f;
	float mouseY = (win.y / 2.0f - ypos) / win.y * 2.0f;

	return glm::vec2(mouseX, mouseY);
}

glm::vec2 Input::GetMousePosition()
{
	double xpos, ypos;
	glfwGetCursorPos(Application::GetWindowHandle(), &xpos, &ypos);
	return glm::vec2((float)xpos, (float)ypos);
}

void Input::SetCursorMode(CursorMode mode)
{
	glfwSetInputMode(Application::GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
}
