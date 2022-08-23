#pragma once
#include "Mesh.h"
#include "Transfrom.h"
#include "Material.h"
#include "Texture.h"
#include "glm/glm.hpp"

class Cuboid : public Transform
{
public:
	Cuboid(float width, float length, float height);
	~Cuboid();

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
	Cube(float x) : Cuboid(x, x, x) { }
	~Cube() { }

	void SetSimpleMaterial(SimpleMaterial material) { m_SimpleMaterial = material; }
	SimpleMaterial GetSimpleMaterial() const { return m_SimpleMaterial; }

	void SetMaterial(Material material) { m_Material = material; }
	Material GetMaterial() const { return m_Material; }

	std::shared_ptr<Texture> GetTexture() const { return m_Material.Texture; }
	std::shared_ptr<Texture> GetSpecularMap() const { return m_Material.SpecularMap; }

private:
	SimpleMaterial m_SimpleMaterial;
	Material m_Material;
};