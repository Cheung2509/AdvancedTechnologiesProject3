#version 460 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 u_MVP;
uniform vec4 u_colour;

out vec4 fragmentColour;

void main()
{
	gl_Position = u_MVP * vec4(vertexPosition_modelspace, 1);
	fragmentColour = u_colour;
};