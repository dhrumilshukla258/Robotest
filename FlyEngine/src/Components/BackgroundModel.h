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

class BackgroundModel : public Component
{
public:
	BackgroundModel();
	~BackgroundModel();
	void Init();
	void SelfDelete();
	void Clone(Component *);

public:
	std::vector<Mesh> mMeshes;
	glm::mat4 * mModelMatrix;
	glm::mat4 * mDebugMatrix;
	glm::mat4 * mBackground_ModelMatrix;
	glm::mat4 * mStencil_Matrix;
	float x_max, x_min, y_max, y_min;
	std::string mModelFile;
	
	aiMatrix4x4 m_global_inverse_transform;

	std::map<std::string, int> m_bone_mapping; // maps a bone name and their index
	int m_num_bones = 0;
	std::vector<BoneMatrix> m_bone_matrices;


	GLuint m_bone_location[100];
	float ticks_per_second = 0.0f;

private:
	RTTR_ENABLE(Component)
};















