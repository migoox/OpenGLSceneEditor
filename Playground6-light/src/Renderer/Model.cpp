#include "Model.h"

void Model::LoadModel(const std::string& filepath)
{
	m_FilePath = filepath;
	Assimp::Importer importer;

	// Scene is a root object in assimp's hierarchy
		// aiProcess_Triangulate forces model to consist only of triangles (converts if it's not)
		// aiProcess_FlipUVs flips the texture coordinates, since images in OpenGL are flipped around y-axis
	const aiScene* scene = importer.ReadFile(m_FilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Other useful options:
		// aiProcess_GenNormals: creates normal vectors for each vertex if the model doesn't contain normal vectors.
		// aiProcess_SplitLargeMeshes: splits large meshes into smaller sub - meshes which is useful if your rendering has a maximum number of vertices allowedand can only process smaller meshes.
		// aiProcess_OptimizeMeshes: does the reverse by trying to join several meshes into one larger mesh, reducing drawing calls for optimization.

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "[Assimp Error]: " << importer.GetErrorString() << "\n";
		return;
	}

	m_Directory = m_FilePath.substr(0, m_FilePath.find_last_of('/')) + '/';

	// scene != NULL and scene->mRootNode != NULL and all flags are 1

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// get meshes from the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	// process current node childs
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0]) // mesh must contain texture coords
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// process indices
		// Assimp's interface defines each mesh as having an array of faces, where each face represents 
		// a single primitive, which in our case (due to the aiProcess_Triangulate option) are always triangles
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	if (mesh->mMaterialIndex >= 0) // if mesh contain material index
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		LoadMaterial(material, aiTextureType_DIFFUSE);
		LoadMaterial(material, aiTextureType_SPECULAR);
	}

	Mesh m;
	m.Set(vertices, indices);
	m_Meshes.push_back(m);
}

void Model::LoadMaterial(aiMaterial* material, aiTextureType type)
{
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		bool found = false;
		material->GetTexture(type, i, &str);

		std::string textureFilePath = m_Directory + str.C_Str();

		if (type == aiTextureType_SPECULAR)
		{
			for (unsigned int i = 0; i < m_Material.SpecularMap.size(); i++)
			{
				if (textureFilePath == m_Material.SpecularMap[i]->GetFilePath())
				{
					found = true;
					break;
				}
			}

			if (!found)
				m_Material.SpecularMap.push_back(std::shared_ptr<Texture>(new Texture(textureFilePath)));
		}
		else
		{
			for (unsigned int i = 0; i < m_Material.Texture.size(); i++)
			{
				if (textureFilePath == m_Material.Texture[i]->GetFilePath())
				{
					found = true;
					break;
				}
			}

			if (!found)
				m_Material.Texture.push_back(std::shared_ptr<Texture>(new Texture(textureFilePath)));
		}
	}
}
