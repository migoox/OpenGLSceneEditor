#pragma once
#include "Mesh.h"
#include "Transfrom.h"
#include "Material.h"
#include "Texture.h"
#include "glm/glm.hpp"

class Cuboid : public Transform
{
public:
	Cuboid() { }
	~Cuboid();

	void Create(float width, float length, float height);

	const Mesh& GetMesh() const { return m_Mesh; }
	
	void SetColor(glm::vec4 color) { m_Color = color; }
	glm::vec4 GetColor() const { return m_Color; }

private:
	Mesh m_Mesh;
	glm::vec4 m_Color;
};

class Cube : public Cuboid
{
public:
	Cube() { }
	~Cube() { }

	void Create(float width, float length, float height) = delete;
	void Create(float x);

	void SetSimpleMaterial(SimpleMaterial material) { m_SimpleMaterial = material; }
	SimpleMaterial GetSimpleMaterial() const { return m_SimpleMaterial; }

	void SetMaterial(Material material) { m_Material = material; }
	Material GetMaterial() const { return m_Material; }

private:
	SimpleMaterial m_SimpleMaterial;
	Material m_Material;
};