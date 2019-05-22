#version 430 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec2 texCoords;
layout ( location = 2 ) in vec4 boneIDs;
layout ( location = 3 ) in vec4 weights;

out vec2 TexCoords;
out vec4 fragColour;
out vec4 oWeights;

const int MAX_BONES = 100;

uniform bool u_useAnimations;
uniform mat4 u_MVP;
uniform mat4 u_Bones[MAX_BONES];

void main()
{	
	mat4 BoneTransform = u_Bones[uint(boneIDs[0])] * weights[0];
	BoneTransform += u_Bones[ uint(boneIDs[1])] * weights[1];
	BoneTransform += u_Bones[uint(boneIDs[2])] * weights[2];
	BoneTransform += u_Bones[uint(boneIDs[3])] * weights[3];

	if(u_useAnimations)
	{
		gl_Position = u_MVP * BoneTransform * vec4(position, 1.0f);
	}
	else
	{
		gl_Position = u_MVP * vec4(position, 1.0f);
	}

    TexCoords = texCoords;
	oWeights = weights;
}