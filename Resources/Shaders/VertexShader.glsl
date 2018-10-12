#version 460 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 vertexColour;

uniform mat4 u_MVP;

out vec4 fragmentColour;

void main()
{
	gl_Position = u_MVP * vec4(vertexPosition_modelspace, 1);
	fragmentColour = vertexColour;
};