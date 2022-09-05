#shader vertex
#version 400 core
layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in vec3 inNormal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_NormalMatrix;

void main()
{
	vec3 Normal;
	vec3 FragmentPosition;

	// fix normals
	Normal = u_NormalMatrix * inNormal;

	// convert to world positions and normal scaling (requires model with smoothed normals)
	FragmentPosition = vec3(u_ModelMatrix * inPosition) + (Normal * 0.001);

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(FragmentPosition, 1.0);
}

#shader fragment
#version 400 core

uniform vec4 u_OutlineColor;

out vec4 FragColor;

void main()
{
	FragColor = u_OutlineColor;
}