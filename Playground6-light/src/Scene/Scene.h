#pragma once
#include <vector>

#include "PropetiesPanel.h"
#include "NodeTreePanel.h"
#include "Layer.h"
#include "FPCamera.h"
#include "Cube.h"
#include "Application.h"
#include "ItemSelection.h"

// application layer type child
class Scene : public Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float dTime) override;
	virtual void OnImGuiRender() override;

private:
	void InitCubes(unsigned int count = 10);
	void InitLights();
	void UpdateCamera(float dTime);
	void UpdateTransform(GLFWwindow* window, Transform* transform, float dt);

private:
	std::vector<Node> m_Nodes;
	UIPropetiesPanel m_PropetiesPanel;
	UINodeTreePanel m_NodeTreePanel;
	ItemSelection m_ItemSelection;

	FPCamera m_Camera;
	bool m_EPressed;
	int m_SelectedIndex;

	bool m_LightBoxes;
	bool m_SelectionVisibility;
	int m_FPS;
};
