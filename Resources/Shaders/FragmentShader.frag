#version 430 core

in vec2 TexCoords;
in vec4 fragmentColour;
in vec4 oWeights;

out vec4 color;

uniform sampler2D texture_diffuse;
uniform bool u_textured;

void main( )
{
	if(u_textured)
	{
		color = vec4( texture( texture_diffuse, TexCoords ));
	}
	else
	{
		float r = abs(1 - min(1, oWeights[0] * (1 - oWeights[3]) + oWeights[3]));
		float g =  abs(1 - min(1, oWeights[1] * (1 - oWeights[3]) + oWeights[3]));
		float b =  abs(1 - min(1, oWeights[2] * (1 - oWeights[3]) + oWeights[3]));
		color = vec4(r,g,b, 1.0);
	}
}