#pragma once
#include <glm/glm.hpp>

#include "KeyCodes.h"

class Input
{
public:
	static bool IsKeyDown(KeyCode keycode);
	static bool IsMouseButtonDown(MouseButton button);

	static glm::vec2 GetWindowMousePosition();
	static glm::vec2 GetMousePosition();

	static void SetCursorMode(CursorMode mode);
};

