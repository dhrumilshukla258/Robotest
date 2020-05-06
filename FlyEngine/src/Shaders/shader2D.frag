/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/
#version 430 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D uniformTexture;

void main()
{
	FragColor = vec4(texture(uniformTexture, TexCoords));
}