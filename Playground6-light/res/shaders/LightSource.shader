#shader vertex
#version 330 core
layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * aPosition;
	TexCoord = aTexCoord;
}

#shader fragment
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 u_LightColor;

void main()
{
	FragColor = u_LightColor;
}