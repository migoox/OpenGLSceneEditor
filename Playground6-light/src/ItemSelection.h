#pragma once
#include "Node.h"
#include "Cuboid.h"
#include "Renderer.h"
class ItemSelection
{
public:
	ItemSelection();
	void OnUpdate(Node& node, Shader& shader);

private:
	Cuboid m_XAxis;
	Cuboid m_YAxis;
	Cuboid m_ZAxis;
};

