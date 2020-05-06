/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#version 430 core

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;    
    float shininess;
}; 




in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;
uniform vec3 viewPos;
uniform Material material;






void main()
{    
   
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
   

	
	 
	

    FragColor = vec4( vec3(texture(material.texture_diffuse1, TexCoords)*0.1), 1.0);


 }












 
      
