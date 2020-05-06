/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/
#version 430 core
layout (location = 0) in vec3 aPos;




layout (location = 5) uniform mat4 model;
layout (location = 6) uniform mat4 view;
layout (location = 7) uniform mat4 projection;



void main()
{
 
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}