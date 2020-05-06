/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Timer.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<Timer>("Timer")
		 .property("mTimeLeft", &Timer::mTimeLeft);
}

Timer::Timer() : mTimeLeft(0.0f)
{
	mComponentName = "Timer";
}

Timer::~Timer()
{
}

void Timer::Init() { }

void Timer::SelfDelete() 
{
	mTimeLeft = 0.0f;
	gpComponentManager->mTimer->Delete(this);
}

void Timer::Clone(Component *rhs1) {
 
	Timer* rhs = static_cast<Timer*>(rhs1);
	mTimeLeft = rhs->mTimeLeft;
}