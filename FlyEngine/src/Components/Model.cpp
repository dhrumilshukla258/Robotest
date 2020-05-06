/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Model.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/GraphicsManager.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<Model>("Model")
		 .constructor()
	.property("Color", &Model::mCol)
		 .property("ModelFile", &Model::mModelFile)
	.property("isLight", &Model::light);
}

extern ResourceManager* gpResourceManager;
extern GraphicsManager* gpGraphicsManager;

Model::Model() : x_max(-999), x_min(999), y_max(-999), y_min(999)
{
	mComponentName = "Model";
	mDebugMatrix = reinterpret_cast<glm::mat4 *> ( _aligned_malloc(sizeof(glm::mat4), 16) );
	mModelMatrix = reinterpret_cast<glm::mat4 *> ( _aligned_malloc(sizeof(glm::mat4), 16) );
	mStencil_Matrix = reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16));
	mColor = reinterpret_cast<glm::vec4 *> (_aligned_malloc(sizeof(glm::vec4), 16));
	*mColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	animation = false;
	light = false;
	scene = nullptr;
	// editor purpose
#ifdef _DEBUG
	isSelected = false;
#endif
}

Model::~Model()
{
	_aligned_free ( mDebugMatrix );
	_aligned_free ( mModelMatrix );
	_aligned_free(mColor);
	_aligned_free(mStencil_Matrix);
	mMeshes.clear();
}

void Model::Init()
{
	*mColor = glm::vec4(mCol.x, mCol.y, mCol.z, 1.0f);
	originalColor = mCol;
	/**mDebugMatrix = glm::mat4(1.0f);*/
	*mModelMatrix = glm::mat4(1.0f);
	*mStencil_Matrix = glm::mat4(1.0f);
	mMeshes.clear();

	mMeshes = gpResourceManager->LoadModel(mModelFile);

	animation = false;
	//////////////////////////////////////////////////
	if (gpGraphicsManager->animation)
	{
		animation = gpGraphicsManager->animation;
		gpGraphicsManager->animation = false;
		m_global_inverse_transform = gpGraphicsManager->m_global_inverse_transform;

		m_bone_mapping = gpGraphicsManager->m_bone_mapping; // maps a bone name and their index
		m_num_bones = gpGraphicsManager->m_num_bones;
		m_bone_matrices = gpGraphicsManager->m_bone_matrices;


		// m_bone_location = gpGraphicsManager->m_bone_location;
		ticks_per_second = gpGraphicsManager->ticks_per_second;

		scene = gpGraphicsManager->pScene;
	}

	//Setting Max and Min value of vertices
	for (unsigned int i = 0; i < mMeshes.size(); ++i) {
		std::vector<Vertex> vertices = mMeshes[i].mVertices;
		for (unsigned int j = 0; j < vertices.size(); ++j) {
				
			if (x_max < vertices[j].Position.x) {
				x_max = vertices[j].Position.x;
			}
			if (x_min > vertices[j].Position.x) {
				x_min = vertices[j].Position.x;
			}
			if (y_max < vertices[j].Position.z) {
				y_max = vertices[j].Position.z;
			}
			if (y_min > vertices[j].Position.z) {
				y_min = vertices[j].Position.z;
			}
			if (z_max < vertices[j].Position.y) {
				z_max = vertices[j].Position.y;
			}								 
			if (z_min > vertices[j].Position.y) {
				z_min = vertices[j].Position.y;
			}
			
		}
	}
	glm::vec4 col = *mColor;
}

void Model::SelfDelete() {
	mMeshes.clear();
	*mModelMatrix = glm::mat4(0.0f);
	x_max = 0.0f;
	x_min = 0.0f;
	y_max = 0.0f;
	y_min = 0.0f;
	mModelFile = "";
	scene = nullptr;
	light = false;
	animation = false;
	*mDebugMatrix = glm::mat4(0);



	//m_bone_mapping.clear();
	//m_num_bones = 0;
//	m_bone_matrices.clear();
    //GLuint m_bone_location[100];		TO DO initialize it properly!!
	ticks_per_second = 0.0f;
	//transforms.clear();
	*mColor=glm::vec4(0.0f);
	mCol = glm::vec3(0.0f);

#ifdef _DEBUG
	isSelected = false;
#endif

	gpComponentManager->mModel->Delete(this);
}

void Model::Clone(Component * rhs1) {
	Model* rhs = static_cast<Model*>(rhs1);
	mMeshes = rhs->mMeshes;
	//glm::mat4 * mModelMatrix;
	/*glm::mat4 * mDebugMatrix;*/
	x_max = rhs->x_max;
	x_min = rhs->x_min;
	y_max = rhs->y_max;
	y_min = rhs->y_min;
	mModelFile = rhs->mModelFile;
	*mColor = *rhs->mColor;
	scene = rhs->scene;
	mCol = rhs->mCol;
	light = rhs->light;

}