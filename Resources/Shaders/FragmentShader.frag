#version 460 core

layout(location = 0) out vec4 colour;

uniform vec4 u_colour;

void main()
{
	colour = u_colour;
}