/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "Sprite.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;
extern ResourceManager * gpResourceManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<Sprite>("Sprite")
		 .property("Texture", &Sprite::texturePath)
		 .property("Shape", &Sprite::shape);
}

Sprite::Sprite() : mTextureID(0), mVAO(0)
{
	mComponentName = "Sprite";
}


Sprite::~Sprite()
{
}

void Sprite::Init()
{
	mTextureID = gpResourceManager->LoadTexture(texturePath);
	mVAO = gpResourceManager->LoadMesh(shape);
}

void Sprite::SelfDelete() {
	mTextureID = 0;
	mVAO = 0;
	texturePath = "";
	shape = "";
	gpComponentManager->mSprite->Delete(this);
}
void Sprite::Clone(Component * rhs1) { 
	Sprite* rhs = static_cast<Sprite*>(rhs1);

	texturePath = rhs->texturePath;
	shape = rhs->shape;
	mVAO = rhs->mVAO;
	mTextureID = rhs->mTextureID;
}