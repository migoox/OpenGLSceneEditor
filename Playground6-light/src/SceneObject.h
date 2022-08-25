#pragma once
#include "imgui/imgui.h"

class SceneObject : public Transform
{
public:
	virtual void ImGuiDrawInfoPanel() { }
	virtual std::string GetName() { return "no-name"; }
};