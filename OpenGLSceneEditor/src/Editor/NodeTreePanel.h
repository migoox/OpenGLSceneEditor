#pragma once
#include <vector>
#include "Node.h"
#include "Panel.h"
#include "imgui/imgui.h"

class UINodeTreePanel : public Panel
{
public:
	void Init();
	void Display(std::vector<Node>& nodes, int &selectedIndex);
private:
	bool m_AddWindowActivated;
	bool m_ButtonDisabled;
};

