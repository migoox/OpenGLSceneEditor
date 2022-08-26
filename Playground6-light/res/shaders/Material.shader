#shader vertex
#version 400 core
layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in vec3 inNormal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_NormalMatrix;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragmentPosition;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * inPosition;
	TexCoords = inTexCoords;

	// fix normals and pass to fragment
	Normal = u_NormalMatrix * inNormal;

	// convert to world positions and pass to fragment 
	FragmentPosition = vec3(u_ModelMatrix * inPosition);
}

#shader fragment
#version 400 core

struct Material
{
	sampler2D diffuse0;
	sampler2D diffuse1;
	sampler2D diffuse2;
	sampler2D specular0;
	sampler2D specular1;
	sampler2D specular2;
	float shininess;
};

struct Light 
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirectionalLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

struct Spotlight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
	float cutOff;
	float outerCutOff;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragmentPosition;

uniform vec3 u_ViewerPosition;
uniform Material u_Material;

#define MAX_DEFAULT_LIGHTS 10
uniform int u_DefaultLightsCount;
uniform Light u_Lights[MAX_DEFAULT_LIGHTS];

#define MAX_DIRECTIONAL_LIGHTS 10
uniform int u_DirectionalLightsCount;
uniform DirectionalLight u_DirectionalLights[MAX_DIRECTIONAL_LIGHTS];

#define MAX_POINT_LIGHTS 10
uniform int u_PointLightsCount;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];

#define MAX_SPOTLIGHTS 10
uniform int u_SpotlightsCount;
uniform Spotlight u_Spotlights[MAX_SPOTLIGHTS];

out vec4 FragColor;

vec3 CalcPhong(vec3 FragToLightDir, vec3 lAmbient, vec3 lDiffuse, vec3 lSpecular);
vec3 CalcDeafultLight(Light light);
vec3 CalcDirectionalLight(DirectionalLight light);
vec3 CalcPointLight(PointLight light);
vec3 CalcSpotlight(Spotlight light);

void main()
{
	vec3 color = vec3(0.0);

	for(int i = 0; i < u_DefaultLightsCount; i++)
	{
		color += CalcDeafultLight(u_Lights[i]);
	}
	for(int i = 0; i < u_DirectionalLightsCount; i++)
	{
		color += CalcDirectionalLight(u_DirectionalLights[i]);
	}
	for(int i = 0; i < u_PointLightsCount; i++)
	{
		color += CalcPointLight(u_PointLights[i]);
	}
	for(int i = 0; i < u_SpotlightsCount; i++)
	{
		color += CalcSpotlight(u_Spotlights[i]);
	}

	FragColor = vec4(color, 1.0);
}

vec3 CalcPhong(vec3 lightDir, vec3 lAmbient, vec3 lDiffuse, vec3 lSpecular)
{
	// ambient lighting
	vec3 ambient = lAmbient * vec3(texture(u_Material.diffuse0, TexCoords));

	// diffuse lighting
	vec3 norm = normalize(Normal);										  // normalize normal in case it isn't normalized

	float diffuseImpact = max(dot(norm, lightDir), 0.0);		      // get similarity of vectors 
																		  // diff <= 0 => opposite direction => no impact
	vec3 diffuse = lDiffuse * diffuseImpact * vec3(texture(u_Material.diffuse0, TexCoords));

	// specular lighting
	vec3 viewDir = normalize(u_ViewerPosition - FragmentPosition);		  // viewer (from fragment towards viewer)
	vec3 reflectDir = -lightDir - 2.0 * dot(-lightDir, norm) * norm;      // reflection vector

	float specularImpact = max(dot(viewDir, reflectDir), 0.0);
																		  // dot gives impact, if it's <= 0
																		  // we set it to 0, then we raising it to 32
																		  // in order to get visible result

	if (specularImpact > 0.0) specularImpact = pow(specularImpact, u_Material.shininess);

	vec3 specular = lSpecular * specularImpact * vec3(texture(u_Material.specular0, TexCoords));

	return (ambient + diffuse + specular);
}

vec3 CalcDeafultLight(Light light)
{
	return CalcPhong(normalize(light.position - FragmentPosition), light.ambient, light.diffuse, light.specular);
}

vec3 CalcDirectionalLight(DirectionalLight light)
{
	return CalcPhong(normalize(-light.direction), light.ambient, light.diffuse, light.specular);
}

vec3 CalcPointLight(PointLight light)
{
	// attenuation
	float d = distance(FragmentPosition, light.position);
	float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * d * d);

	return CalcPhong(normalize(light.position - FragmentPosition), light.ambient, light.diffuse, light.specular) * attenuation;
}

vec3 CalcSpotlight(Spotlight light)
{
	// ambient lighting
	vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse0, TexCoords));

	// spotlight 
	float cosTheta = dot(normalize(FragmentPosition - light.position), normalize(light.direction));
	float cosCutOff = cos(light.cutOff);
	float cosEpsilon = cosCutOff - cos(light.outerCutOff);

	if (cosTheta < cosCutOff)
	{
		return ambient;
	}
	float intensity = clamp((cosTheta - cosCutOff) / cosEpsilon, 0.f, 1.f);

	// diffuse lighting
	vec3 norm = normalize(Normal);										 
	vec3 lightDir = normalize(light.position - FragmentPosition);		  

	float diffuseImpact = max(dot(norm, lightDir), 0.0);				  

	vec3 diffuse = light.diffuse * diffuseImpact * vec3(texture(u_Material.diffuse0, TexCoords));

	// specular lighting
	vec3 viewDir = normalize(u_ViewerPosition - FragmentPosition);
	vec3 reflectDir = -lightDir - 2 * dot(-lightDir, norm) * norm;		  

	float specularImpact = max(dot(viewDir, reflectDir), 0.0);

	if (specularImpact > 0.0) specularImpact = pow(specularImpact, u_Material.shininess);

	vec3 specular = light.specular * specularImpact * vec3(texture(u_Material.specular0, TexCoords));

	return ambient + intensity * diffuse + intensity * specular;
}