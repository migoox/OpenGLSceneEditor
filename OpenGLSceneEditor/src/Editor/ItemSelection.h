#pragma once
#include "Node.h"
#include "Cube.h"
#include "Renderer.h"
class ItemSelection
{
public:
	ItemSelection();
	void OnUpdate(Node& node);

private:
	Cube m_XAxis;
	Cube m_YAxis;
	Cube m_ZAxis;
};

