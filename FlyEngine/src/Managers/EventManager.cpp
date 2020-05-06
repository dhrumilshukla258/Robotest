/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "GameplayManager.h"

extern EntityManager* gpEventManager;
extern GameplayManager* gpManager;
extern SystemManager* gpSystemManager;

Event::Event(EventType type) : mTimer(0.0f)
{
	mType = type;
}

EventManager::EventManager() 
{
	LOG_ADD("EventManager");
}

EventManager::~EventManager() 
{
	for (auto block : mBlock) {
		delete[] block.second;
	}

	for (auto list : mEventMemory) {
		list.second.clear();
	}
	mEvents.clear();
	mEventMemory.clear();
}

void EventManager::AddEvent(Event * _pEvent, unsigned int _id)
{
	std::pair<Event*, unsigned int> t = std::make_pair(_pEvent, _id);
	mEvents.push_back(t);
}

void EventManager::Update(float _frameTime) {
	//Loop Through all time events
	//Decrease timer by Frametime
	// < 0 ? -> fire event, Remove from list
	
	auto itEnd = mEvents.end();
	auto it = mEvents.begin();

	while (it != itEnd) {
		Event *pEvent = it->first;
		unsigned int id = it->second;
		pEvent->mTimer -= _frameTime;
		if (pEvent->mTimer < 0.0f) {
			gpManager->HandleEvent(pEvent, id);
			it = mEvents.erase(it);
			mEventMemory[pEvent->mType].push_front(pEvent);
		}
		else
			++it;
	}
	
}