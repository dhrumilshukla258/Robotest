#pragma once
#include "../Shaders/Shader.h"
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "../Shaders/Mesh.h"

using namespace std;

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

	void Update();
	void Update2D();
	void Update3D_Model(Shader ourShader);
	//void Update3D_Background_Model(Shader ourShader);

	void Update3D_Stencil_Toon_Model_Stencil(Shader ourShader);
	void Update3D_Stencil_Toon_Model(Shader ourShader);

	
	void Update3D_Debug(Shader ourShader);
	void Update3D_Raw(Shader ourShader);
	void Update_Particle(Shader ourShader);

	void Use_Shader3D(Shader ourShader);
	void Use_Shader3D_Stencil_Toon(Shader ourShader);
	void Use_Shader3D_Debug(Shader ourShader);
	void Use_Shader3D_Raw(Shader ourShader);
	void Use_Shader3D_Shadow(Shader ourShader);
	void End_Use_Shader3D_Shadow();
	void Use_Shader_Particle(Shader ourShader);


	void Use_Stencil_Shader(Shader ourShader);
	

	void Update_Lights(Shader ourShader);
	


	////////////////

public:
	unsigned int VAO;
	unsigned int VAO_HDR;
	unsigned int VAO_Quad;

	bool debug_Select;

	

	////////////////////////////////////
///////Temporary camera
	//////////////////////

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	glm::mat4 * projection3D;
	glm::mat4 * projection_light;
	glm::mat4 * lightView;
	glm::mat4 * shadow_matrix;

	////////////////////////////
	float x = 0.0f, y = 0.0f, z = 0.0f;
	float lx = 0.0f, ly = 0.0f, lz = 0.0f;	//////////////////
};


