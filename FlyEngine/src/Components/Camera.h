#pragma once
#include "../Component.h"

class Camera : public Component
{
public:
	Camera();
	~Camera();
	void Init() {}
	void SelfDelete();
	void Clone(Component *);

public:
	glm::vec3 mMinBound;
	glm::vec3 mMaxBound;
	bool keepFollow;

private:
	RTTR_ENABLE(Component)
};

