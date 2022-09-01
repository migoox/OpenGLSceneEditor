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
	vec3 ambient;
	int noDiffTexture;
	vec3 diffuse;
	sampler2D diffuse0;
	sampler2D diffuse1;
	sampler2D diffuse2;

	int noSpecMap;
	vec3 specular;
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

vec3 CalcPhong(vec3 FragToLightDir, vec3 lAmbient, vec3 lDiffuse, vec3 lSpecular, vec3 matAmb, vec3 matDiff, vec3 matSpec);
vec3 CalcDeafultLight(Light light, vec3 matAmb, vec3 matDiff, vec3 matSpec);
vec3 CalcDirectionalLight(DirectionalLight light, vec3 matAmb, vec3 matDiff, vec3 matSpec);
vec3 CalcPointLight(PointLight light, vec3 matAmb, vec3 matDiff, vec3 matSpec);
vec3 CalcSpotlight(Spotlight light, vec3 matAmb, vec3 matDiff, vec3 matSpec);

void main()
{
	vec3 color = vec3(0.0);
	vec3 matAmb, matDiff, matSpec;

	if (u_Material.noDiffTexture != 0)
	{
		matDiff = u_Material.diffuse;
		matAmb = u_Material.ambient;
	}
	else
	{
		matDiff = vec3(texture(u_Material.diffuse0, TexCoords));
		matAmb = matDiff;
	}

	if (u_Material.noSpecMap != 0)
		matSpec = u_Material.specular;
	else
		matSpec = vec3(texture(u_Material.specular0, TexCoords));

	for(int i = 0; i < u_DefaultLightsCount; i++)
	{
		color += CalcDeafultLight(u_Lights[i], matAmb, matDiff, matSpec);
	}
	for(int i = 0; i < u_DirectionalLightsCount; i++)
	{
		color += CalcDirectionalLight(u_DirectionalLights[i], matAmb, matDiff, matSpec);
	}
	for(int i = 0; i < u_PointLightsCount; i++)
	{
		color += CalcPointLight(u_PointLights[i], matAmb, matDiff, matSpec);
	}
	for(int i = 0; i < u_SpotlightsCount; i++)
	{
		color += CalcSpotlight(u_Spotlights[i], matAmb, matDiff, matSpec);
	}

	FragColor = vec4(color, 1.0);
}

vec3 CalcPhong(vec3 lightDir, vec3 lAmbient, vec3 lDiffuse, vec3 lSpecular, vec3 matAmb, vec3 matDiff, vec3 matSpec)
{
	// ambient lighting
	vec3 ambient = lAmbient * matAmb;

	// diffuse lighting
	vec3 norm = normalize(Normal);										  // normalize normal in case it isn't normalized

	float diffuseImpact = max(dot(norm, lightDir), 0.0);		      // get similarity of vectors 
																		  // diff <= 0 => opposite direction => no impact
	vec3 diffuse = lDiffuse * diffuseImpact * matDiff;

	// specular lighting
	vec3 viewDir = normalize(u_ViewerPosition - FragmentPosition);		  // viewer (from fragment towards viewer)
	vec3 reflectDir = -lightDir - 2.0 * dot(-lightDir, norm) * norm;      // reflection vector

	float specularImpact = max(dot(viewDir, reflectDir), 0.0);
																		  // dot gives impact, if it's <= 0
																		  // we set it to 0, then we raising it to 32
																		  // in order to get visible result

	if (specularImpact > 0.0) specularImpact = pow(specularImpact, u_Material.shininess);

	vec3 specular = lSpecular * specularImpact * matSpec;

	return (ambient + diffuse + specular);
}

vec3 CalcDeafultLight(Light light, vec3 matAmb, vec3 matDiff, vec3 matSpec)
{
	return CalcPhong(normalize(light.position - FragmentPosition), light.ambient, light.diffuse, light.specular, matAmb, matDiff, matSpec);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 matAmb, vec3 matDiff, vec3 matSpec)
{
	return CalcPhong(normalize(-light.direction), light.ambient, light.diffuse, light.specular, matAmb, matDiff, matSpec);
}

vec3 CalcPointLight(PointLight light, vec3 matAmb, vec3 matDiff, vec3 matSpec)
{
	// attenuation
	float d = distance(FragmentPosition, light.position);
	float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * d * d);

	return CalcPhong(normalize(light.position - FragmentPosition), light.ambient, light.diffuse, light.specular, matAmb, matDiff, matSpec) * attenuation;
}

vec3 CalcSpotlight(Spotlight light, vec3 matAmb, vec3 matDiff, vec3 matSpec)
{
	// ambient lighting
	vec3 ambient = light.ambient * matAmb;

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

	vec3 diffuse = light.diffuse * diffuseImpact * matDiff;

	// specular lighting
	vec3 viewDir = normalize(u_ViewerPosition - FragmentPosition);
	vec3 reflectDir = -lightDir - 2 * dot(-lightDir, norm) * norm;		  

	float specularImpact = max(dot(viewDir, reflectDir), 0.0);

	if (specularImpact > 0.0) specularImpact = pow(specularImpact, u_Material.shininess);

	vec3 specular = light.specular * specularImpact * matSpec;

	return ambient + intensity * diffuse + intensity * specular;
}