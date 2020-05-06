/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Oscillation.h"
#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<Oscillation>("Oscillation")
		 .constructor()
		 .property("Start", &Oscillation::mStart)
		.property("ResetBoundaries", &Oscillation::mReset)
		 .property("Velocity", &Oscillation::mVelocity)
		 .property("LeftRightDistance", &Oscillation::mLeftRightDistance)
		 .property("UpDownDistance", &Oscillation::mUpDownDistance)
		 .property("LPos", &Oscillation::LPos)
		 .property("UPos", &Oscillation::UPos)
	     .property("idealVel", &Oscillation::idealVel)
	;


}

Oscillation::Oscillation()
{
	mComponentName = "Oscillation";
}

void Oscillation::Init() {
	if (mReset) {
		glm::vec3 iniPos = mpOwner->GetComponent<Transform>()->mPos;
		
		
		LPos = glm::vec3(iniPos.x - fabs(mLeftRightDistance) / 2.0f, iniPos.y - fabs(mUpDownDistance) / 2.0f, iniPos.z);
		UPos = glm::vec3(iniPos.x + fabs(mLeftRightDistance) / 2.0f, iniPos.y + fabs(mUpDownDistance) / 2.0f, iniPos.z);
		idealVel = mVelocity;
		mReset = false;
	}
}

Oscillation::~Oscillation()
{
}

void Oscillation::SelfDelete() {

	mLeftRightDistance = 0.0f;
	mUpDownDistance = 0.0f;
	mVelocity = glm::vec2(0, 0);
	idealVel = mVelocity;	mReset = false;
	gpComponentManager->mOscillation->Delete(this);
}

void Oscillation::Clone(Component * rhs1) {
	Oscillation* rhs = static_cast<Oscillation*>(rhs1);
	mStart = rhs->mStart;
	mReset = rhs->mReset;
	LPos = rhs->LPos;
	UPos = rhs->UPos;
	idealVel = rhs->idealVel;
	mVelocity = rhs->mVelocity;
	initPosition = rhs->initPosition;
	mLeftRightDistance = rhs->mLeftRightDistance;
	mUpDownDistance = rhs->mUpDownDistance;

	
	mVelocity = rhs->mVelocity;
	idealVel = rhs->idealVel;
	mReset = false;

	//bool mStart = false;
	//bool mReset = false;// for when relocating the position of a pltform. can recalculate  boundaries
	//glm::vec3 LPos, UPos;
	//glm::vec2 idealVel;
	//glm::vec2 mVelocity;
	//glm::vec3 initPosition;
	//float tempUpDown = 0.0f;
	//float tempLeftRight = 0.0f;
}