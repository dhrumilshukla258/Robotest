/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 5) uniform mat4 model;
layout (location = 7) uniform mat4 projection;
layout (location = 8) uniform mat4 shadowMatrix;


out vec2 TexCoords;
out vec3 ourColor;
out vec4 vshadow_position;

void main()
{
	gl_Position = projection * model * vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoords = vec2( aTexCoord.x, aTexCoord.y );
	vshadow_position =	shadowMatrix  * model * vec4(aPos, 1.0);
}