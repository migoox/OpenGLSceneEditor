#pragma once
#include "Mesh.h"
#include "Transfrom.h"
#include "Material.h"
#include "glm/glm.hpp"

class Cube : public Transform
{
public:
	Cube();
	~Cube();

	const Mesh& GetMesh() const { return m_Mesh; }
	
	void SetMaterial(Material material) { m_Material = material; }
	Material GetMaterial() const { return m_Material; }

	void SetColor(glm::vec4 color) { m_Color = color; }
	glm::vec4 GetColor() const { return m_Color; }

private:
	Mesh m_Mesh;
	glm::vec4 m_Color;
	Material m_Material;
};
