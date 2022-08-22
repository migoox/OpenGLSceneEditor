#pragma once
#include "Mesh.h"
#include "Transfrom.h"
#include "Material.h"
#include "glm/glm.hpp"

class Cuboid : public Transform
{
public:
	Cuboid(float width, float length, float height);
	~Cuboid();

	inline const Mesh& GetMesh() const { return m_Mesh; }
	inline void SetColor(glm::vec4 color) { m_Color = color; }
	inline void SetMaterial(Material material) { m_Material = material; }
	inline glm::vec4 GetColor() const { return m_Color; }
	inline Material GetMaterial() const { return m_Material; }

private:
	Mesh m_Mesh;
	Material m_Material;
	glm::vec4 m_Color;
};