/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#pragma once
#include "../Shaders/Shader.h"

#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Shaders/Mesh.h"


class Mesh;

struct ModelInstance
{
	glm::mat4 ModelMatrix;
};


class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();
	int ShaderInitialize(std::string, std::string);
	unsigned int CreateRekt();
	unsigned int CreateRekt_Particle();
	unsigned int CreateTriangle();
	unsigned int CreateCircle();
	unsigned int CreateLine();
	unsigned int LoadTextureCall(std::string);
	void Frame_Bubber_Set();
	std::vector<Mesh> LoadModelCall(std::string);
	void showNodeName(aiNode* node);

	void ProcessNode(aiNode*, const aiScene*, std::string);
	Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene, std::string _directory);

	unsigned int Create_Frame_Buffer_Pointer();
	unsigned int CreateQuad();
public:
	Shader shader2D;
	Shader shader3D;
	Shader shader3D_Debug;
	Shader shader3D_Shadow;
	Shader shader3D_Raw;
	Shader shader_Particle;
	Shader shader_Stencil;
	Shader shader_HDR;
	Shader shader_Blur;
	std::vector<Mesh> meshes;
	std::vector<aiMatrix4x4> transforms;
	GLuint frame_buffer, frame_buffer_texture_buffer;
	
	aiMatrix4x4 m_global_inverse_transform;
	std::map<std::string, int> m_bone_mapping; // maps a bone name and their index
	int m_num_bones = 0;
	std::vector<BoneMatrix> m_bone_matrices;
	GLuint m_bone_location[100];
	float ticks_per_second = 0.0f;
	aiScene* pScene;
	bool animation = false;
	

	float* pMatrix_particles;

	 float* pColor_particles;
	 GLuint vertexMatrix;
	GLuint vertexColor;
	unsigned int amount = 3000;
	std::vector<glm::mat4> modelMatrices;
	std::vector<glm::vec4> color_Vec4;

	unsigned int colorBuffers[2];
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];
	

	unsigned int rboDepth;

	unsigned int quadVAO ;
	unsigned int quadVBO;
	unsigned int hdrFBO;

};