#shader vertex
#version 330 core
layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_NormalMatrix;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragmentPosition;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * inPosition;
	TexCoord = inTexCoord;

	// fix normals and pass to fragment
	Normal = u_NormalMatrix * inNormal;

	// convert to world positions and pass to fragment 
	FragmentPosition = vec3(u_ModelMatrix * inPosition);
}

#shader fragment
#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragmentPosition;

uniform vec3 u_ViewerPosition;
uniform Material u_Material;
uniform Light u_Light;

out vec4 FragColor;

void main()
{
	// ambient lighting
	vec3 ambient = u_Light.ambient * u_Material.ambient;

	// diffuse lighting
	vec3 norm = normalize(Normal);									// normalize normal in case it isn't normalized
	vec3 lightDir = normalize(u_Light.position - FragmentPosition);  // get direction from object (fragment) towards light

	float diffuseImpact = max(dot(norm, lightDir), 0.0);			// get similarity of vectors 
																	// diff <= 0 => opposite direction => no impact
	vec3 diffuse = u_Light.diffuse * (diffuseImpact * u_Material.diffuse);

	// specular lighting
	float specularLightingStrenghtConst = 0.5;

	vec3 viewDir = normalize(u_ViewerPosition - FragmentPosition);		  // viewer (from fragment towards viewer)
	vec3 reflectDir = -lightDir - 2 * dot(-lightDir, norm) * norm;		  // reflection vector

	float specularImpact = max(dot(viewDir, reflectDir), 0.0);
																		  // dot gives impact, if it's <= 0
																		  // we set it to 0, then we raising it to 32
																		  // in order to get visible result
	
	if (specularImpact > 0.0) specularImpact = pow(specularImpact, u_Material.shininess);

	vec3 specular = u_Light.specular * (specularImpact * u_Material.specular);

	FragColor = vec4(ambient + diffuse + specular, 1.f);
}