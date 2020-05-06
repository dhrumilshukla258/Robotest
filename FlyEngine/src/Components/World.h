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
#include "Light.h"

//inline unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class World : public Component
{
public:
	World();
	~World();
	void Init();
	void SelfDelete();
public:
	std::list<Light*> light_list;
	Light* light_Closest;

	

	
private:
	RTTR_ENABLE(Component)
};