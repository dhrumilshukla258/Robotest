#pragma once

#include "../Component.h"

class Transform : public Component
{
public:
	Transform();
	~Transform();
	void Init();
	void SelfReset();
	void Clone(Component *);

	void SelfDelete();
public:
	glm::vec3 mPos;
	glm::vec3 mScale;
	glm::vec3 mRotationDir;
	glm::vec3 mChildOffset;
	float mRotate; // rotation value in degrees
	bool mDirtyFlag;

	
private:
	RTTR_ENABLE(Component)
};