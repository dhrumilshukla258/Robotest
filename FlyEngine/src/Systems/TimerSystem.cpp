/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "TimerSystem.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/ObjectFactory.h"
#include "../Managers/SystemManager.h"
#include "../Systems/PlayerSystem.h"

extern ComponentManager* gpComponentManager;
extern ObjectFactory* gpObjectFactory;
extern SystemManager* gpSystemManager;

TimerSystem::TimerSystem()
{
}

TimerSystem::~TimerSystem()
{
}

void TimerSystem::Update(float _frameTime)
{
	Component* c = gpComponentManager->mTimer->GetHead();
	while (nullptr != c) {
		Timer* pT = static_cast<Timer*>(c);
		pT->mTimeLeft -= _frameTime;
		if (pT->mTimeLeft <= 0.0f) {
			gpObjectFactory->DeleteObject(pT->mpOwner->id);
		}
		c = c->GetNext();
	}
}