/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Homing.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<Homing>("Homing")
		 .property("Speed", &Homing::mSpeed)
		 .property("RotSpeed", &Homing::mRotSpeed)
		 .property("InitialPadding", &Homing::mInitialPadding)
		 .property("HomingActive", &Homing::mHomingActive)
		 .property("KeepHoming", &Homing::mKeepHoming);
}

Homing::Homing() : mTarget(0.0f), mSpeed(0.0f), mRotSpeed(0.0f), mInitialPadding(0.0f), mHomingActive(true), mKeepHoming(true)
{
	mComponentName = "Homing";
}

Homing::~Homing()
{
}

void Homing::Init()
{
}

void Homing::SelfDelete() {
	mTarget = glm::vec2(0.0f);
	mSpeed = 0.0f;
	mRotSpeed = 0.0f;
	gpComponentManager->mHoming->Delete(this);
	mInitialPadding = 0.0f;
	mKeepHoming = true;
	mHomingActive = true;
}
void Homing::Clone(Component* rhs1) {
	Homing* rhs = static_cast<Homing*>(rhs1);

	mTarget = rhs->mTarget;
	mSpeed =rhs->mSpeed;
	mRotSpeed = rhs->mRotSpeed;
	mInitialPadding = rhs->mInitialPadding;
}