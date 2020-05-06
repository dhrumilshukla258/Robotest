/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view_model;

void main()
{
    gl_Position = projection * view_model * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}