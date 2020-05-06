#pragma once
#include "../Component.h"

enum DoorType {
	PressOnce,
	PressConstantly,
	PressMultipleTimes
};

class Door : public Component
{
public:
	Door();
	~Door();
	void Init();
	void SelfDelete();
	void Clone(Component *);

public:
	bool mKeyHit = false;
	bool mPrevKeyHit = false;
	bool mKeyForMultipleHit = false;
	DoorType mDoorType;

	bool isOpen = false;
	bool isClose = true;

	float doorLength = 0.0f;
	float tempDoorLength = 0.0f;

	Entity* triggerToClose = nullptr;
	bool wantToClose = false;
	bool closeDoor = false;
private:
	RTTR_ENABLE(Component)
};