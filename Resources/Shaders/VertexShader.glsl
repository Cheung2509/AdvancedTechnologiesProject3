#version 410 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec2 texCoords;
layout ( location = 3 ) in uvec4 boneIDs;
layout ( location = 4 ) in vec4 weights;

out vec2 TexCoords;

const int MAX_BONES = 100;

uniform bool u_useAnimations;
uniform mat4 u_MVP;
uniform mat4 u_Bones[MAX_BONES];

void main()
{
	mat4 BoneTransform = mat4(1.0);

	if(u_useAnimations)
	{
		BoneTransform = u_Bones[boneIDs[0]] * weights[0];
		BoneTransform += u_Bones[boneIDs[1]] * weights[1];
		BoneTransform += u_Bones[boneIDs[2]] * weights[2];
		BoneTransform += u_Bones[boneIDs[3]] * weights[3];
	}
	
	gl_Position = u_MVP * BoneTransform * vec4(position, 1.0f);

    TexCoords = texCoords;
}