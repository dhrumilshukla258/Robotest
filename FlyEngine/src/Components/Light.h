#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Shaders/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Shaders/Mesh.h"
#include "../Shaders/Shader.h"
#include "../Component.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

//inline unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
enum Light_type
{
	POINT_LIGHT,
	SPOT_LIGHT,
	DIRECTIONAL_LIGHT
};

class Light : public Component
{
public:
	Light();
	~Light();
	void Init();
	void SelfDelete();
	void Clone(Component *);
public:
	
	int type;

	glm::vec3 point_Light_Position;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 direction;
private:
	RTTR_ENABLE(Component)
};