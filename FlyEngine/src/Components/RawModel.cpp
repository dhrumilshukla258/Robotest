/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "RawModel.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/GraphicsManager.h"
#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<RawModel>("RawModel")
		 .constructor()
		 .property("RawModelFile", &RawModel::mModelFile);
}

extern ResourceManager* gpResourceManager;
extern GraphicsManager* gpGraphicsManager;

RawModel::RawModel() 
{
	mComponentName = "RawModel";
}

RawModel::~RawModel()
{
	mMeshes.clear();
}

void RawModel::Init()
{
	mMeshes.clear();
	mMeshes = gpResourceManager->LoadModel(mModelFile);
	
	// Editor purpose
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
}

void RawModel::SelfDelete() {
	mMeshes.clear();
	mModelFile = "";
	gpComponentManager->mRawModel->Delete(this);
}

void RawModel::Clone(Component * rhs1) {
	RawModel* rhs = static_cast<RawModel*>(rhs1);
	mMeshes = rhs->mMeshes;
	mModelFile = rhs->mModelFile;
}