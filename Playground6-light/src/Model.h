#pragma once
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Material.h"
#include "Transfrom.h"

class Model : public Transform
{
public:
	Model() { }

	void LoadModel(const std::string& filepath);

	std::vector<Mesh>& GetMeshes() { return m_Meshes; }
	Material GetMaterial() { return m_Material; }

private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterial(aiMaterial* material, aiTextureType type);

private:
	std::string m_FilePath;
	std::string m_Directory;

	std::vector<Mesh> m_Meshes;
	Material m_Material;
};

