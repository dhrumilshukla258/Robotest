/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "ParticleGenerator.h"
#include "../Managers/GraphicsManager.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

extern GraphicsManager* gpGraphicsManager;
extern ResourceManager * gpResourceManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<ParticleGenerator>("ParticleGenerator")
	.constructor();
	//.property("Texture", &ParticleGenerator::texturePath);

	
}

ParticleGenerator::ParticleGenerator()
{
	mComponentName = "ParticleGenerator";
}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::Init()
{
	mVAO = gpResourceManager->LoadMesh("Rekt_Part");

	  vertexMatrix = gpGraphicsManager->vertexMatrix;
	 vertexColor = gpGraphicsManager->vertexColor;
	 model_Matrix = gpGraphicsManager->modelMatrices;
	 color_Particles = gpGraphicsManager->color_Vec4;
	//mTextureID = gpResourceManager->LoadTexture(texturePath);

}

void ParticleGenerator::SelfDelete() {
	for (Particle_Struct* temp : particle_list) {
		
		temp->~Particle_Struct();
	}


	particle_list.clear();
	
	mVAO = 0;

	mTextureID = 0;

	gpComponentManager->mParticleGenerator->Delete(this);
}

void ParticleGenerator::Clone(Component* rhs1) {
	ParticleGenerator* rhs = static_cast<ParticleGenerator*>(rhs1);

	//std::list<Particle*> particle_list;
	 texturePath = rhs->texturePath;
	 mVAO = rhs->mVAO;
	 mTextureID = rhs->mTextureID;

}