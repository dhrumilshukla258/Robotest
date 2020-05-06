/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "Transform.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/SystemManager.h"
#include "../Systems/PlayerSystem.h"
#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

extern SystemManager* gpSystemManager;
using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<Transform>("Transform")
		 .property("Position", &Transform::mPos)
		 .property("Scale", &Transform::mScale)
		 .property("Rotation", &Transform::mRotate)
		 .property("RotationDir", &Transform::mRotationDir)
		 .property("ChildOffset", &Transform::mChildOffset);
		
}

Transform::Transform() : mPos(0.0f), mChildOffset(0.0f), mRotate(0.0f), mRotationDir(0.0f, 1.0f, 0.0f), mDirtyFlag(true)
{
	mComponentName = "Transform";
	mpOwner = nullptr;
	mScale = glm::vec3(0.001f, 0.001f, 0.001f);
	
}

Transform::~Transform() {}

void Transform::Init() {
}

void Transform::SelfReset()
{
	mpOwner = nullptr;
	mPos = glm::vec3(0.0f);
	mScale = glm::vec3(0.001f, 0.001f, 0.001f);
	mChildOffset = glm::vec3(0);
	mRotate = 0.0f;
	
	mDirtyFlag = true;
}

void Transform::SelfDelete() {
	SelfReset();
	gpComponentManager->mTransform->Delete(this);
}
void Transform::Clone(Component* rhs1) {
	//mpOwner = nullptr;
	Transform* rhs = static_cast<Transform*>(rhs1);
	mPos = rhs->mPos;
	mScale = rhs->mScale;//glm::vec3(0.001f, 0.001f, 0.001f);
	mChildOffset = rhs->mChildOffset;// glm::vec3(0);
	mRotate = rhs->mRotate;// 0.0f;

	mDirtyFlag = true;
}