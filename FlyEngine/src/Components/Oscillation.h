#pragma once
#include "../Component.h"

class Oscillation : public Component
{
public:
	Oscillation();
	~Oscillation();
	void SelfDelete();
	void Clone(Component * );

	void Init();
public:
	bool mStart = false;
	bool mReset = false;// for when relocating the position of a pltform. can recalculate  boundaries
	glm::vec2 mVelocity;
	float mLeftRightDistance = 0.0f;
	float mUpDownDistance = 0.0f;
	glm::vec3 LPos, UPos;
	glm::vec2 idealVel;
	glm::vec3 initPosition;
private:
	RTTR_ENABLE(Component)
};