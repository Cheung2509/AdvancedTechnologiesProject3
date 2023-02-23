#version 430 core

in vec2 TexCoords;
in vec3 normal;
in vec4 fragmentColour;
in vec4 oWeights;

out vec4 color;

uniform sampler2D u_texture;
uniform int u_renderType;

void main( )
{
	if(u_renderType == 0)
	{
		color = vec4(texture(u_texture, TexCoords));
	}
	else if(u_renderType == 1)
	{
		float r = abs(1 - min(1, oWeights[0] * (1 - oWeights[3]) + oWeights[3]));
		float g = abs(1 - min(1, oWeights[1] * (1 - oWeights[3]) + oWeights[3]));
		float b = abs(1 - min(1, oWeights[2] * (1 - oWeights[3]) + oWeights[3]));
		color = vec4(r, g, b, 1.0);
	}
	else if(u_renderType == 2)
	{
		color = vec4(abs(normalize(normal)), 1.0);
	}
}