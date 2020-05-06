/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Stencil_Toon_Model.h"

#include "../Managers/ResourceManager.h"
#include "../Managers/GraphicsManager.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<Stencil_Toon_Model>("Stencil_Toon_Model")
	 .constructor()
	 .property("Color", &Stencil_Toon_Model::mCol)
	 //.property("isLight", &Model::isLight)
	 .property("Stencil_Toon_ModelFile", &Stencil_Toon_Model::mModelFile);
}

extern ResourceManager* gpResourceManager;
extern GraphicsManager* gpGraphicsManager;

Stencil_Toon_Model::Stencil_Toon_Model() : x_max(-999), x_min(999), y_max(-999), y_min(999)
{
	mComponentName = "Stencil_Toon_Model";
	mDebugMatrix = reinterpret_cast<glm::mat4 *> ( _aligned_malloc(sizeof(glm::mat4), 16) );
	mModelMatrix = reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16));
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

Stencil_Toon_Model::~Stencil_Toon_Model()
{
	_aligned_free ( mDebugMatrix );
	_aligned_free(mModelMatrix);
	_aligned_free(mStencil_Matrix);
	_aligned_free(mColor);
	mMeshes.clear();
}

void Stencil_Toon_Model::Init()
{
	*mColor = glm::vec4(mCol.x,mCol.y,mCol.z,1.0f);
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
		m_global_inverse_transform = gpGraphicsManager->m_global_inverse_transform;

		m_bone_mapping = gpGraphicsManager->m_bone_mapping; // maps a bone name and their index
		m_num_bones =  gpGraphicsManager->m_num_bones;
		m_bone_matrices=gpGraphicsManager->m_bone_matrices;


		// m_bone_location = gpGraphicsManager->m_bone_location;
		ticks_per_second = gpGraphicsManager->ticks_per_second;

		scene = gpGraphicsManager->pScene;
	}
	
	 
	 //////////////////////////////////////

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
		}
	}
}

void Stencil_Toon_Model::SelfDelete() {
	mMeshes.clear();
	*mModelMatrix = glm::mat4(0.0f);
	scene = nullptr;
	x_max = 0.0f;
	x_min = 0.0f;
	y_max = 0.0f;
	y_min = 0.0f;
	mModelFile = "";
	gpComponentManager->mStencil_Toon_Model->Delete(this);
}
