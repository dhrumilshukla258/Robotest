#pragma once
#include "../Component.h"

class Sprite;
class Transform;
class Model;

class Body : public Component
{
public:
	Body();
	~Body();

	//void Serialize(GenericObject<false, Value::ValueType> doc);
	void Init();
	void Initialize();
	void SelfReset();
	void SelfDelete();
	void Clone(Component *);
	void Move(float);

public:
	glm::vec2 mPos;
	glm::vec2 mPrevPos;
	glm::vec2 mVel;
	glm::vec2 mAcc;
	glm::vec2 mTotalForce;

	glm::vec2 constantVelocity;

	float mMass, mInvMass;

	bool applyFriction;
	bool isConstantVelocitySet;
	bool constrainX, constrainY;
	bool useGravity;

	void SetConstantVelocity(glm::vec2 vel);

	// Component pointers
	Transform* pTr;
	Model* pModel;
	Sprite* debugSprite;

private:
	RTTR_ENABLE(Component)
};

