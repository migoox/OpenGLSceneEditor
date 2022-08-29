#pragma once
#include "imgui/imgui.h"

class Layer
{
protected:
	bool m_Visible;
public:
	Layer() : m_Visible(true) { }

	virtual ~Layer() = default;

	virtual void OnAttach() { }
	virtual void OnDetach() { }

	virtual void OnUpdate(float dTime) { }

	virtual void OnImGuiRender() { }

	bool IsVisible() { return m_Visible; }
};