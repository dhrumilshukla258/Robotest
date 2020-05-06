/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Health.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<Health>("Health")
		 .property("Health", &Health::mHealthLeft);
}

Health::Health() : mHealthLeft(0.0f)
{
	mComponentName = "Health";
}

Health::~Health()
{
}

void Health::Init()
{
}

void Health::SelfDelete() {
	mHealthLeft = 0.0f;
	gpComponentManager->mHealth->Delete(this);
}

void Health::Clone(Component  *rhs1) {
	Health* rhs = static_cast<Health*>(rhs1);

	 mHealthLeft = rhs->mHealthLeft;
}