#pragma once
#include "Node.h"
#include "Panel.h"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"

class UIPropetiesPanelTransformSection
{
public:
	void Display(Node& node);

private:
	float m_Position[3];
	float m_Eulers[3];
	float m_Scale[3];
};

class UIPropetiesPanelCubeSection
{
public:
	void Init();
	void Display(Node& node);

private:
	size_t m_CubeHash;
	std::string textruePath;
	std::string mapPath;
};

class UIPropetiesPanelModelSection
{
public:
	void Init();
	void Display(Node& node);
private:
	size_t m_ModelHash;
};


class UIPropetiesPanelLightSection
{
public:
	void Init();
	void Display(Node& node);

private:
	size_t m_LightHash;
	size_t m_DirLightHash;
	size_t m_PointLightHash;
	size_t m_SpotlightHash;

	float m_Ambient[3];
	float m_Diffuse[3];

	float m_Specular;

	float m_Constant;
	float m_Linear;
	float m_Quadratic;

	float m_CutOffAngle;
	float m_OuterCutOffAngle;
};

class UIPropetiesPanel : public Panel
{
public:
	void Init();
	void DisplayBlank();
	void Display(Node& node);

private:
	UIPropetiesPanelTransformSection m_TransformSection;
	UIPropetiesPanelCubeSection m_CubeSection;
	UIPropetiesPanelModelSection m_ModelSection;
	UIPropetiesPanelLightSection m_LightSection;
};