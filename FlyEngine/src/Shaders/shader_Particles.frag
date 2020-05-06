/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#version 430 core
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 FragColor;

uniform sampler2D sprite;

void main()
{
     vec4 texColor = texture(sprite, TexCoords)* ParticleColor;
    if(texColor.a < 0.005)
        discard;
	FragColor = texColor;
	//FragColor = vec4(1,0,0,1);
}  


