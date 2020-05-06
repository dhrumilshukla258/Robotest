/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/
#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>

typedef unsigned int uint;
#define NUM_BONES_PER_VEREX 100

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
};
struct BoneMatrix
{
	aiMatrix4x4 offset_matrix;
	aiMatrix4x4 final_world_transform;

};
struct VertexBoneData
{
	uint ids[NUM_BONES_PER_VEREX];   // we have 4 bone ids for EACH vertex & 4 weights for EACH vertex
	float weights[NUM_BONES_PER_VEREX];

	VertexBoneData()
	{
		memset(ids, 0, sizeof(ids));    // init all values in array = 0
		memset(weights, 0, sizeof(weights));
	}

	void addBoneData(int bone_id, float weight);
};

class Mesh {
public:
	Mesh( std::vector<Vertex>, std::vector<unsigned int>, std::vector<Texture> , std::vector<VertexBoneData> bone_id_weights);
	~Mesh();
	void Draw(Shader);
private:
	void SetupMesh();
public:
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::vector<Texture> mTextures;
	std::vector<VertexBoneData> bones_id_weights_for_each_vertex;

	unsigned int VAO;
private:
	unsigned int VBO, EBO;
	GLuint VBO_bones;
};