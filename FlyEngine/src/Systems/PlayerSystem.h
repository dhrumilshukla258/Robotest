#pragma once
#include "PlayerStates.h"
#include "../Entity.h"

enum Directions {
	North = 1,
	West = 2,
	South = 4,
	East = 8,
	NorthWest = 3,
	NorthEast = 9,
	SouthWest = 6,
	SouthEast = 12,
	DirectionsMaxSum = 15
};

class PlayerSystem
{
public:
	Body* pBody;
public:
	PlayerSystem();
	~PlayerSystem();

	IdleState idleState;
	RunningState runState;
	JumpingState jumpState;
	GrabbingState grabbingState;
	FallingState fallState;

	void UpdatePlayerAimDirection();
	void HandleInput();
	void Update(float);
	void ResetToPosition(glm::vec2 pos);
	void AddCheckpoint(Entity*);
	void ShowDeathAnimation();
	void ShowRebirthAnimation();

	void SetPlayerEntity(Entity* en);
	void SetFireGun(Entity* en);
	void SetIceGun(Entity* en);
	Entity* GetPlayerEntity();
	Entity* GetFireGun();
	Entity* GetIceGun();

private:
	PlayerState* _state;
	Entity* playerEntity;
	Entity* fireGun;
	Entity* iceGun;
	glm::vec3 mFireScale;
	glm::vec3 mIceScale;
public:
	// player direction
	// aim direction
	bool isGrounded;
	bool hasDied;

	glm::vec2 offset = {0.0f, 0.0f};

	Directions aimDirection;
	float cosAngle;
	float sinAngle;
	float mBulletTime = 0.0f;
	Directions playerDirection;
	
	glm::vec3 mCheckpoint;
	std::vector<glm::vec3> mCpVec;
	
	Body* mFireBody;
	Body* mIceBody;
	Transform* mFireTransform;
	Transform* mIceTransform;

	bool isFire = false;
	bool isIce = false;
private:
	float FlipPlayer(Directions dir);
	tweeny::tween<float> tweenL;
	tweeny::tween<float> tweenR;
};

