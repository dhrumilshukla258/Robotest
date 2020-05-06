/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#pragma once

class Hit;
enum EventType {
	COLLIDE,
	HIT
};

class Event
{
public:
	Event(EventType);
	virtual void Update(unsigned int _id) {}
	virtual Event* GetSelf() = 0;
	virtual ~Event() {}
public:
	EventType mType;
	float mTimer;
};

class EventManager {
public:
	EventManager();
	~EventManager();

	template <typename T, unsigned int N>
	void RegisterEvent(EventType _type) 
	{
		T * x = new T[N];
		mBlock[_type] = x;
		for (int i = 0; i < N; ++i) {
			mEventMemory[_type].push_front(&x[i]);
		}
	}

	Event* GetEvent(EventType _type) 
	{
		Event* x = mEventMemory[_type].front();
		mEventMemory[_type].pop_front();
		return x;
	}
	
	void AddEvent(Event* _type, unsigned int _id);
	void Update(float _frameTime);
public:
	std::list< std::pair<Event*, unsigned int> > mEvents;
private:
	std::unordered_map< unsigned int, Event* > mBlock;
	std::unordered_map< unsigned int, std::forward_list<Event*> > mEventMemory;
};