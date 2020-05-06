#pragma once
#include "../Component.h"

class Timer : public Component
{
public:
	Timer();
	~Timer();
	void Init();
	void SelfDelete();
	void Clone(Component *);

public:
	float mTimeLeft;
private:
	RTTR_ENABLE(Component)
};

