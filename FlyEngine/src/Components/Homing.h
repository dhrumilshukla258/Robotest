#pragma once
#include "../Component.h"

class Homing : public Component
{
public:
	Homing();
	~Homing();
	void Init();
	void SelfDelete();
	void Clone(Component *);


public:
	glm::vec2 mTarget;
	float mSpeed;
	float mRotSpeed;
	float mInitialPadding;
	bool mHomingActive;
	bool mKeepHoming;
private:
	RTTR_ENABLE(Component)
};