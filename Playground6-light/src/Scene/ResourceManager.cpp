#include "ResourceManager.h"
std::unordered_map<std::string, std::string> ResourceManager::s_TextureAliasToPath;
std::unordered_map<std::string, std::shared_ptr<Texture>> ResourceManager::s_PathToTexture;

std::unordered_map<std::string, std::string> ResourceManager::s_ShaderAliasToPath;
std::unordered_map<std::string, std::shared_ptr<Shader>> ResourceManager::s_PathToShader;

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& alias_or_path)
{
	std::string path;
	auto findPath = s_TextureAliasToPath.find(alias_or_path);

	if (findPath != s_TextureAliasToPath.end())
		path = findPath->second;
	else
		path = alias_or_path;

	auto findTexture = s_PathToTexture.find(path);

	if (findTexture != s_PathToTexture.end())
		return findTexture->second;

	std::cout << "[Resource Manager]: " << "Loading texture: " << path << "...\n";
	auto newTexture = std::shared_ptr<Texture>(new Texture(path));

	if (!newTexture->IsLoadingSucceded())
	{
		std::cout << "[Resource Manager]: " << "Error: Can't load texture with path: " << path << "\n";
	}
	else
	{
		s_PathToTexture.insert(std::make_pair(path, newTexture));
		std::cout << "[Resource Manager]: " << "Texture: " << path << " has been loaded successfully." << "\n";
	}

	return newTexture;
}

void ResourceManager::AddTextureAlias(const std::string& alias, const std::string& path)
{
	auto findPath = s_TextureAliasToPath.find(alias);
	if (findPath != s_TextureAliasToPath.end())
	{
		std::cout << "[Resource Manager Warning]: " << "Alias's texture path has been changed: " << alias << ", " << findPath->second << " -> " << alias << ", " << path << "\n";

	}
	s_TextureAliasToPath.insert(std::make_pair(alias, path));
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& alias_or_path)
{
	std::string path;
	auto findPath = s_ShaderAliasToPath.find(alias_or_path);

	if (findPath != s_ShaderAliasToPath.end())
		path = findPath->second;
	else
		path = alias_or_path;

	auto findShader = s_PathToShader.find(path);

	if (findShader != s_PathToShader.end())
		return findShader->second;
	
	std::cout << "[Resource Manager]: " << "Loading shader: " << path << "...\n";
	auto newShader = std::shared_ptr<Shader>(new Shader(path));

	if (!newShader->IsLoadingSucceded())
	{
		std::cout << "[Resource Manager]: " << "Error: Can't load shader with path: " << path << "\n";
	}
	else
	{
		s_PathToShader.insert(std::make_pair(path, newShader));
		std::cout << "[Resource Manager]: " << "Shader: " << path << " has been loaded successfully."<< "\n";

	}

	return newShader;
}

void ResourceManager::AddShaderAlias(const std::string& alias, const std::string& path)
{
	auto findPath = s_ShaderAliasToPath.find(alias);
	if (findPath != s_ShaderAliasToPath.end())
	{
		std::cout << "[Resource Manager]: " << "Warning: Alias's shader path has been changed: " << alias << ", " << findPath->second << " -> " << alias << ", " << path << "\n";

	}
	s_ShaderAliasToPath.insert(std::make_pair(alias, path));
}
