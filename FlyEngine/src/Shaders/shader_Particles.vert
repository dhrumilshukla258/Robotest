/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/
#version 430 core

layout (location = 0) in vec3 aPos;

layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instMatrix;
layout (location = 7) in vec4 col;


layout (location = 8) uniform mat4 view;
layout (location = 9) uniform mat4 projection;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 instanceMatrix[200];
uniform vec4 color[200];



void main()
{
 /// mat4 model = instanceMatrix[gl_InstanceID];
    TexCoords = aTexCoord;
   // ParticleColor = color[gl_InstanceID];
	 ParticleColor = col;
   gl_Position = projection * view * instMatrix * vec4(aPos, 1.0);
}