#version 460 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(vertexPosition_modelspace, 1);
};