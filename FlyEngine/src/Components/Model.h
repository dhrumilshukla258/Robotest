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
#include "glm\gtc\quaternion.hpp"

#include <string>
using namespace std;

//inline unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);


class Model : public Component
{
public:
	Model();
	~Model();
	void Init();
	void SelfDelete();
	void Clone(Component * );

public:
	static const int MAX_BONES = 100;

	std::vector<Mesh> mMeshes;
	glm::mat4 * mModelMatrix;
	glm::mat4 * mDebugMatrix;
	glm::mat4 * mStencil_Matrix;
	float x_max, x_min, y_max, y_min, z_max, z_min;
	std::string mModelFile;
	

	aiMatrix4x4 m_global_inverse_transform;

	std::map<std::string, int> m_bone_mapping; // maps a bone name and their index
	int m_num_bones = 0;
	std::vector<BoneMatrix> m_bone_matrices;


	GLuint m_bone_location[100];
	float ticks_per_second = 0.0f;

	aiScene* scene;

	bool animation = false;

	std::vector<aiMatrix4x4> transforms;
	glm::vec4* mColor;
	glm::vec3 mCol;
	glm::vec3 originalColor;
	bool light;

	// editor purpose
//#ifdef _DEBUG
	bool isSelected;
//#endif

private:
	RTTR_ENABLE(Component)
};