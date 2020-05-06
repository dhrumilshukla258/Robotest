#pragma once
#include "../Component.h"
class Entity;
class Boss : public Component
{
public:
	Boss();
	~Boss();
	void Init();
	void SelfDelete();
	void Clone(Component *);
	void DeleteChildReference(unsigned int);
public:
	std::vector<Entity*> mMinions;
	std::vector<Entity*> mDrones;
	Entity* mHealthBar = nullptr;
	Entity* mExpression = nullptr;
	Entity* mDoor = nullptr;
	bool killedAllLauncher = false;
	int launcherDestroyed = 0;
	bool playForwardAnimation = false;
	bool playBackwardAnimation = false;
private:
	bool visitedOnce = false;
protected:
	RTTR_ENABLE(Component)
};

