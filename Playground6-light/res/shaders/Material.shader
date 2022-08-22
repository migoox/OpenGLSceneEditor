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
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light 
{
	int type; // 0 - deafult light, 1 - directional light, 2 - point light, 3 - spotlight

	// for all types
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// default light, point light, spotlight (0, 2, 3)
	vec3 position;

	// directional light, spotlight (1, 3)
	vec3 direction;

	// point light (2)
	float constant;
	float linear;
	float quadratic;

	// spotlight (3)
	float cutOff;
	float outerCutOff;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragmentPosition;

uniform vec3 u_ViewerPosition;
uniform Material u_Material;
uniform Light u_Light;

out vec4 FragColor;

vec4 DeafultLight();
vec4 DirectionalLitght();
vec4 PointLight();
vec4 Spotlight();

void main()
{
	if (u_Light.type == 1)
	{
		FragColor = DirectionalLitght();
	}
	else if (u_Light.type == 2)
	{
		FragColor = PointLight();
	}
	else if (u_Light.type == 3)
	{
		FragColor = Spotlight();
	}
	else
	{
		FragColor = DeafultLight();
	}
}

vec4 DeafultLight()
{
	// ambient lighting
	vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse, TexCoords));

	// diffuse lighting
	vec3 norm = normalize(Normal);										  // normalize normal in case it isn't normalized
	vec3 lightDir = normalize(u_Light.position - FragmentPosition);		  // get direction from object (fragment) towards light

	float diffuseImpact = max(dot(norm, lightDir), 0.0);				  // get similarity of vectors 
																		  // diff <= 0 => opposite direction => no impact
	vec3 diffuse = u_Light.diffuse * diffuseImpact * vec3(texture(u_Material.diffuse, TexCoords));

	// specular lighting
	float specularLightingStrenghtConst = 0.5;

	vec3 viewDir = normalize(u_ViewerPosition - FragmentPosition);		  // viewer (from fragment towards viewer)
	vec3 reflectDir = -lightDir - 2 * dot(-lightDir, norm) * norm;		  // reflection vector

	float specularImpact = max(dot(viewDir, reflectDir), 0.0);
	// dot gives impact, if it's <= 0
	// we set it to 0, then we raising it to 32
	// in order to get visible result

	if (specularImpact > 0.0) specularImpact = pow(specularImpact, u_Material.shininess);

	vec3 specular = u_Light.specular * specularImpact * vec3(texture(u_Material.specular, TexCoords));

	return vec4(ambient + diffuse + specular, 1.f);
}

vec4 DirectionalLitght()
{
	// ambient lighting
	vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse, TexCoords));

	// diffuse lighting
	vec3 norm = normalize(Normal);										  // normalize normal in case it isn't normalized
	vec3 lightDir = normalize(-u_Light.direction);						  // get direction from object (fragment) towards light

	float diffuseImpact = max(dot(norm, lightDir), 0.0);				  // get similarity of vectors 
																		  // diff <= 0 => opposite direction => no impact
	vec3 diffuse = u_Light.diffuse * diffuseImpact * vec3(texture(u_Material.diffuse, TexCoords));

	// specular lighting
	float specularLightingStrenghtConst = 0.5;

	vec3 viewDir = normalize(u_ViewerPosition - FragmentPosition);		  // viewer (from fragment towards viewer)
	vec3 reflectDir = -lightDir - 2 * dot(-lightDir, norm) * norm;		  // reflection vector

	float specularImpact = max(dot(viewDir, reflectDir), 0.0);
	// dot gives impact, if it's <= 0
	// we set it to 0, then we raising it to 32
	// in order to get visible result

	if (specularImpact > 0.0) specularImpact = pow(specularImpact, u_Material.shininess);

	vec3 specular = u_Light.specular * specularImpact * vec3(texture(u_Material.specular, TexCoords));

	return vec4(ambient + diffuse + specular, 1.f);
}

vec4 PointLight()
{
	// ambient lighting
	vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse, TexCoords));

	// diffuse lighting
	vec3 norm = normalize(Normal);										  // normalize normal in case it isn't normalized
	vec3 lightDir = normalize(u_Light.position - FragmentPosition);		  // get direction from object (fragment) towards light

	float diffuseImpact = max(dot(norm, lightDir), 0.0);				  // get similarity of vectors 
																		  // diff <= 0 => opposite direction => no impact
	vec3 diffuse = u_Light.diffuse * diffuseImpact * vec3(texture(u_Material.diffuse, TexCoords));

	// specular lighting
	float specularLightingStrenghtConst = 0.5;

	vec3 viewDir = normalize(u_ViewerPosition - FragmentPosition);		  // viewer (from fragment towards viewer)
	vec3 reflectDir = -lightDir - 2 * dot(-lightDir, norm) * norm;		  // reflection vector

	float specularImpact = max(dot(viewDir, reflectDir), 0.0);
	// dot gives impact, if it's <= 0
	// we set it to 0, then we raising it to 32
	// in order to get visible result

	if (specularImpact > 0.0) specularImpact = pow(specularImpact, u_Material.shininess);

	vec3 specular = u_Light.specular * specularImpact * vec3(texture(u_Material.specular, TexCoords));

	// attenuation
	float d = distance(FragmentPosition, u_Light.position);
	float attenuation = 1.0 / (u_Light.constant + u_Light.linear * d + u_Light.quadratic * d * d);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return vec4(ambient + diffuse + specular, 1.f);
}

vec4 Spotlight()
{
	// ambient lighting
	vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse, TexCoords));

	// spotlight 
	float cosTheta = dot(normalize(FragmentPosition - u_Light.position), normalize(u_Light.direction));
	float cosCutOff = cos(u_Light.cutOff);
	float cosEpsilon = cosCutOff - cos(u_Light.outerCutOff);

	if (cosTheta < cosCutOff)
	{
		return vec4(ambient, 1.f);
	}
	float intensity = clamp((cosTheta - cosCutOff) / cosEpsilon, 0.f, 1.f);

	// diffuse lighting
	vec3 norm = normalize(Normal);										  // normalize normal in case it isn't normalized
	vec3 lightDir = normalize(u_Light.position - FragmentPosition);		  // get direction from object (fragment) towards light

	float diffuseImpact = max(dot(norm, lightDir), 0.0);				  // get similarity of vectors 
																		  // diff <= 0 => opposite direction => no impact
	vec3 diffuse = u_Light.diffuse * diffuseImpact * vec3(texture(u_Material.diffuse, TexCoords));

	// specular lighting
	float specularLightingStrenghtConst = 0.5;

	vec3 viewDir = normalize(u_ViewerPosition - FragmentPosition);		  // viewer (from fragment towards viewer)
	vec3 reflectDir = -lightDir - 2 * dot(-lightDir, norm) * norm;		  // reflection vector

	float specularImpact = max(dot(viewDir, reflectDir), 0.0);
	// dot gives impact, if it's <= 0
	// we set it to 0, then we raising it to 32
	// in order to get visible result

	if (specularImpact > 0.0) specularImpact = pow(specularImpact, u_Material.shininess);

	vec3 specular = u_Light.specular * specularImpact * vec3(texture(u_Material.specular, TexCoords));

	return vec4(ambient + intensity * diffuse + intensity * specular, 1.f);
}