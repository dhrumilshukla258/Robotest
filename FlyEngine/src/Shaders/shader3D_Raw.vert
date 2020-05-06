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


uniform mat4 normal_matrix;


void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	 FragPos = vec3(model * vec4(aPos,1.0));
	// light_vector = light_position - (model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;  
	 vshadow_position =	shadowMatrix  * model * vec4(aPos, 1.0);
	 //gl_Position=vshadow_position;
}