#pragma once
#include "glm/glm.hpp"
#include "Texture.h"
#include <memory>

struct Material
{
	std::vector<std::shared_ptr<Texture>> SpecularMap;
	std::vector<std::shared_ptr<Texture>> Texture;
	float Shininess;
};

struct SimpleMaterial
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};


class SimpleMaterials
{
public:
	enum class Type { Emerald = 0, Ruby = 1, Gold = 2, Turquoise = 3, Pearl = 4 };
	static SimpleMaterial GetMaterial(Type type)
	{
		if(type == Type::Emerald)
			return { glm::vec3(0.0215f, 0.1745f, 0.0215f),
					 glm::vec3(0.07568f, 0.61424f, 0.07568f),
					 glm::vec3(0.633f, 0.727811f, 0.633f), 0.6f * 128 };

		else if(type == Type::Ruby)
			return { glm::vec3(0.1745f, 0.01175f, 0.01175f),
					 glm::vec3(0.61424f, 0.04136f, 0.04136f),
					 glm::vec3(0.727811f, 0.626959f, 0.626959f), 0.6f * 128 };

		else if (type == Type::Gold)
			return { glm::vec3(0.24725f, 0.1995f, 0.0745f),
					 glm::vec3(0.75164f, 0.60648f, 0.22648f),
					 glm::vec3(0.628281f, 0.555802f, 0.366065f), 0.4f * 128 };


		else if (type == Type::Turquoise)
			return { glm::vec3(0.1f, 0.18725f, 0.1745f),
					 glm::vec3(0.396f, 0.74151f, 0.69102f),
					 glm::vec3(0.297254f, 0.30829f, 0.306678f),  12.8f };

		// Pearl
		return { glm::vec3(0.25f, 0.20725f, 0.20725f),
				 glm::vec3(1.f, 0.829f, 0.829f),
				 glm::vec3(0.296648f, 0.296648f, 0.296648f), 0.088f * 128 };
	}
	
};
