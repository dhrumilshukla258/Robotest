#pragma once
#include "../Managers/InputManager.h"
#include "../Managers/SystemManager.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/AudioManager.h"
#include "../Components/Body.h"
#include "../Components/Animation.h"
#include "tweeny/tweeny.h"
extern SystemManager * gpSystemManager;
extern ComponentManager * gpComponentManager;
extern AudioManager * gpAudioManager;


class PlayerState {
public:
	Body* playerBody;
	Animation* pAnim;
	
	
	PlayerState() {
		playerBody = nullptr;
	}
	virtual ~PlayerState() {}
	virtual void Enter();
	virtual void Exit() {}
	virtual void Update();
	virtual PlayerState* HandleInput(InputManager* input);
};




class IdleState : public PlayerState {
public:
	void Enter();
	PlayerState* HandleInput(InputManager* input);
	void Update();
};

class RunningState : public PlayerState {
public:
	float forceX;
	bool isRunningRight;
	bool isRunningLeft;

	void Enter();
	PlayerState* HandleInput(InputManager* input);

	void Update();
};

class GrabbingState : public PlayerState {
public:
	GrabbingState();
	bool isPushing;

	Body* grabbedBody;
	float pushingForce;
	int GrabbingChannel;
	bool grabbing = false;
	void Update();
	void Enter();
	void Exit();
	PlayerState* HandleInput(InputManager* input);
};

class JumpingState : public PlayerState {
public:
	bool jumpingUp;
	bool jumpingDown;

	JumpingState();

	float forceY;
	float forceAirX;
	
	bool isJumpPressed;

	float jumpTime;
	void Enter();
	void Update();

	PlayerState* HandleInput(InputManager* input);

	void Exit();
};

class FallingState : public PlayerState 
{
	float forceAirX;

	void Enter();
	void Update();

	PlayerState* HandleInput(InputManager* input);

	void Exit();
public:
	FallingState();
	bool isFalling;
};