/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#version 430 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;    
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;
in vec4 vshadow_position;
uniform vec3 viewPos;
uniform Material material;

uniform sampler2D shadow_sampler;
uniform int total_Point_Light;
uniform int total_Spot_Light;
uniform int total_Directional_Light;

uniform DirLight dirLight[5];
uniform PointLight pointLights[50];
uniform SpotLight spotLight[50];
uniform vec3 closestLight;
uniform vec3 player_pos;
uniform vec4 _Color = vec4 (1.0,1.0,1.0,1) ;
uniform vec4 _UnlitColor = vec4 (0.2,0.2,0.2,1) ;
uniform float _DiffuseThreshold=0.1;
 
    
uniform vec4	_SpecColor  = vec4 (1,1,1,1) ;
   uniform float   _Shininess = 10.0;

uniform vec4 _OutlineColor  = vec4(1.5,1.5,1.5,1);

uniform vec3 camera_pos;
         
uniform float _LitOutlineThickness  = 0.1;
uniform float _UnlitOutlineThickness   = 0.25;
uniform int stencil_toon=0;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

float Shadow_Calculation(DirLight light, vec3 normal, vec3 viewDir,vec4 vshadow_pos);

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcPointLight_Stencil(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight_Stencil(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
uniform vec4 Color_Model;

uniform int light_on;

void main()
{    
   
    vec3 normalDirection = normalize(Normal);
	vec3 vertexToLightSource = closestLight-FragPos;
    vec3 viewDirection = normalize(viewPos - FragPos);
   vec3 lightDirection = FragPos -closestLight;
   vec3 color;
   float intensity;
   

	  float distance = length(closestLight - FragPos);
  
   float attenuation = 1.0 / (1.0 + 0.027f * 2 + 0.0028f * (2* 2));   

	

  
   intensity = dot(closestLight,normalDirection);
	
	
	
	
	
 vec3 result= vec3(0);


	//result=ambient;

	vec3 fragmentColor= vec3(0);
	


	

		for(int i = 0; i < total_Directional_Light; i++)
		{
		
			result +=  CalcDirLight(dirLight[i], normalDirection, viewDirection);
			
			
		
		}
	
		for(int i = 0; i < total_Point_Light; i++)
		{
		
			result += CalcPointLight(pointLights[i], normalDirection, FragPos, viewDirection);
			
			
		
		}
		for(int i = 0; i < total_Spot_Light; i++)
		{
			result += CalcSpotLight(spotLight[i], normalDirection, FragPos, viewDirection);  
		}


		if(light_on==1)
		{
				 
			
				BrightColor = vec4(result*Color_Model.xyz, 1.0);
			//	vec3 temp = normalize(0.1);	
			    FragColor = vec4(result, 1.0);
		}
		else
		{
			

		
				float brightness = dot(result*Color_Model.xyz, vec3(0.2126, 0.7152, 0.0722));
					if(brightness > 1.0)
					{
					 vec3 temp = normalize(result*Color_Model.xyz);
						BrightColor = vec4(temp, 1.0);
					}
					else
					{
						BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
					}
						
			

					
						
					// vec3 temp = normalize(Color_Model.xyz);

						FragColor = vec4(result*Color_Model.xyz, 1.0);
		}
 
		
		
	

		


 }



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
 vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse =  diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
	 float shadow = Shadow_Calculation(light, normal,  viewDir, vshadow_position);           
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse));    
    return (lighting);
}  


float Shadow_Calculation(DirLight light, vec3 normal, vec3 viewDir,vec4 vshadow_pos)
{

  
    vec3 projCoords = vshadow_position.xyz / vshadow_position.w;
      projCoords = projCoords * 0.5 + 0.5;
 
    float closestDepth = texture(shadow_sampler, projCoords.xy).r; 
 
    float currentDepth = projCoords.z;
   
  //  float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
////////////////////////////////////////////////////////////////////////////
  // vec3 lightDir = normalize(light.direction - FragPos);
	 
	 float bias = max(0.05 * (1.0 - dot( normalize(Normal),normalize(-light.direction))), 0.0005);
	
  // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
        

		float shadow = 0.0;
   vec2 texelSize = 1.0 / textureSize(shadow_sampler, 0);
    for(int x = -1; x <= 1; ++x)
    {
       for(int y = -1; y <= 1; ++y)
        {
           float pcfDepth = texture(shadow_sampler, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
  
    if(projCoords.z > 1.0)
        shadow = 0.0;
   return shadow;


	
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
   


	 vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse);
}



vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse);
}

vec3 CalcPointLight_Stencil(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.01);
    // attenuation
    float distance = length(light.position - fragPos);
  
  
 // float attenuation = 1.0 / (1.0 + 0.14 * distance + 0.07 * (distance * distance));    
   float attenuation = 1.0 / (1.0 + 0.027f * distance + 0.0028f * (distance * distance));    
    // combine results
    vec3 ambient = vec3(0.2,0.2,0.2) * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = vec3(1.0,1.0,1.0) * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular =spec* vec3(0.05,0.05,0.05) ;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse);
}



vec3 CalcSpotLight_Stencil(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = spec* vec3(0.05,0.05,0.05);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse);
}








 
      
