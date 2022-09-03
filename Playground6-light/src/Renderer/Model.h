#pragma once
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Material.h"
#include "Transfrom.h"
#include "ResourceManager.h"

class Model : public Transform
{
public:
	Model() : m_LoadingSuccess(false) { }

	void LoadModel(const std::string& filepath);

	std::vector<Mesh>& GetMeshes() { return m_Meshes; }
	Material GetMaterial() { return m_Material; }
	void SetMaterial(Material material) { m_Material = material; }

	bool LoadingSucceded() { return m_LoadingSuccess; }

	std::string GetFilePath() { return m_FilePath; }

private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterial(aiMaterial* material, aiTextureType type);

private:
	std::string m_FilePath;
	std::string m_Directory;

	std::vector<Mesh> m_Meshes;
	Material m_Material;

	bool m_LoadingSuccess;
};

