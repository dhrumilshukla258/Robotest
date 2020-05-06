#pragma once
#include "../Component.h"
class Shape;
class Transform;
class Body;

class Collider : public Component
{
public:
	Collider();
	~Collider();
	void Init();
	void SelfDelete();
	void Clone(Component *);


public:
	Shape* mpShape;
	float coeffRestitution, coeffStaticFriction, coeffDynamicFriction;
	bool isTrigger;
	glm::mat4 * mDebugMatrix;
	Transform* mpTr;
	Body* mpBody;

private:
	RTTR_ENABLE(Component)
};

