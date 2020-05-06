/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout(location = 3) in ivec4 bone_ids;     // INT pointer
layout(location = 4) in vec4 weights;

layout (location = 5) uniform mat4 model;
layout (location = 6) uniform mat4 view;
layout (location = 7) uniform mat4 projection;
layout (location = 8) uniform mat4 shadowMatrix;
uniform vec4 light_position;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 vshadow_position;

//out vec4 light_vector;

uniform int total_Bones;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];
uniform mat4 normal_matrix;
uniform int animation_State;

void main()
{
    if(animation_State==1)
	{
		mat4 bone_transform = mat4(0);
		for(int i=0;i<total_Bones;++i)
		{
			bone_transform += bones[bone_ids[i]] * weights[i];
		}
		gl_Position = projection * view * model *bone_transform* vec4(aPos, 1.0);
	}
	else
	{
		 gl_Position = projection * view * model * vec4(aPos, 1.0);
	}
   
	
}