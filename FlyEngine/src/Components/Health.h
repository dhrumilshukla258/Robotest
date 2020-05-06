#pragma once
#include "../Component.h"

class Health : public Component
{
public:
	Health();
	~Health();
	void Init();
	void SelfDelete();
	void Clone(Component *);

public:
	float mHealthLeft;
private:
	RTTR_ENABLE(Component)
};

