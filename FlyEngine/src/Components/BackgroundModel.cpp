/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "BackgroundModel.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/GraphicsManager.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

RTTR_REGISTRATION
{
	rttr::registration::class_<BackgroundModel>("BackgroundModel")
		 .constructor()
		 .property("BackgroundModelFile", &BackgroundModel::mModelFile);
}

extern ResourceManager* gpResourceManager;
extern GraphicsManager* gpGraphicsManager;

BackgroundModel::BackgroundModel()
{
	mComponentName = "BackgroundModel";
	mDebugMatrix = reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16));
	mModelMatrix = reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16));
	mBackground_ModelMatrix = reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16));
	mStencil_Matrix = reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16));
}

BackgroundModel::~BackgroundModel()
{
	_aligned_free(mDebugMatrix);
	_aligned_free(mModelMatrix);
	_aligned_free(mBackground_ModelMatrix);
	_aligned_free(mStencil_Matrix);
	mMeshes.clear();
}

void BackgroundModel::Init()
{
	*mDebugMatrix = glm::mat4(1.0f);
	*mModelMatrix = glm::mat4(1.0f);
	*mBackground_ModelMatrix = glm::mat4(1.0f);
	mMeshes.clear();

	mMeshes = gpResourceManager->LoadModel(mModelFile);
}

void BackgroundModel::SelfDelete() {
	gpComponentManager->mBackground_Model->Delete(this);
}

void BackgroundModel::Clone(Component * rhs1) 
{
	BackgroundModel* rhs = static_cast<BackgroundModel*>(rhs1);
	mDebugMatrix = rhs->mDebugMatrix;
	mModelMatrix = rhs->mModelMatrix;
	mBackground_ModelMatrix = rhs->mBackground_ModelMatrix;
	mMeshes.clear();
	mMeshes = rhs->mMeshes;
}
