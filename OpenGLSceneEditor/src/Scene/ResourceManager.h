#pragma once
#include "Texture.h"
#include "Shader.h"
#include <cassert>
#include <memory>
#include <unordered_map>

class ResourceManager
{
public:
	static std::shared_ptr<Texture> GetTexture(const std::string& alias_or_path);
	static void AddTextureAlias(const std::string& alias, const std::string& path);

	static std::shared_ptr<Shader> GetShader(const std::string& alias_or_path_or_path);
	static void AddShaderAlias(const std::string& alias, const std::string& path);


private:
	static std::unordered_map<std::string, std::string> s_TextureAliasToPath;
	static std::unordered_map<std::string, std::shared_ptr<Texture>> s_PathToTexture;

	static std::unordered_map<std::string, std::string> s_ShaderAliasToPath;
	static std::unordered_map<std::string, std::shared_ptr<Shader>> s_PathToShader;
};

