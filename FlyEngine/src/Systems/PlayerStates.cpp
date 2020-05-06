/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Shashwat Pandey, shashwat.pandey, 60003718
Author: Ivan Cazares, ivan.c, 60004418
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "PlayerSystem.h"
#include "PlayerStates.h"
#include "../Managers/ObjectFactory.h"
#include "../Managers/SystemManager.h"
#include "../Managers/FrameRateController.h"
#include "../Managers/SceneManager.h"
#include "../Managers/AudioManager.h"


#define PI 3.14159265358f

extern ObjectFactory* gpObjectFactory;
extern SystemManager* gpSystemManager;
extern SceneManager* gpSceneManager;
extern FrameRateController* gpFrameRateController;

float RUN_FORCE = 1500.0f; // change in velocity per frame
float JUMP_FORCE = 40000.0f; // instantaneous force
float AIR_CONTROL_FORCE = 2000.0f;
float FALLING_FORCE = -300.0f;
float VELOCITY_CLAMP = 30.0f;
float RUN_SPEED = 30.0f;
float WALK_SPEED = 20.0f;
float GRAB_WALK_SPEED = 5.0f;
float EXTRA_JUMP = 1820.0f;
float JUMP_HOLD_TIME = 0.3f;

float fallStateEnterValue = -25.0f;

float DistanceCheck(glm::vec2, glm::vec2);

PlayerState* PlayerState::HandleInput(InputManager* input) {
	if (input->isTriggered(X) || input->isTriggered(RCTRL)) {
	
		Entity* mEntity = nullptr;
		Body* pBody_gun = nullptr;

		//if (gpSystemManager->mPlayerSystem->isFire) {
		//	mEntity = gpObjectFactory->CreateRuntimeObject("FireBullet.json");
		//	pBody_gun = gpSystemManager->mPlayerSystem->mFireBody;
		//}
		//if (gpSystemManager->mPlayerSystem->isIce) {
		//	mEntity = gpObjectFactory->CreateRuntimeObject("FireBullet.json");
		//	pBody_gun = gpSystemManager->mPlayerSystem->mIceBody;
		//}
		
		if ( ( gpSystemManager->mPlayerSystem->isFire || gpSystemManager->mPlayerSystem->isIce ) && ( gpSystemManager->mPlayerSystem->mBulletTime <= 0.0f ) )
		{
			mEntity = gpObjectFactory->CreateRuntimeObject("FireBullet.json");
			pBody_gun = gpSystemManager->mPlayerSystem->mFireBody;

			gpSystemManager->mPlayerSystem->mBulletTime = 0.5f;

			Body* pBody_Bullet = mEntity->GetComponent<Body>();
			Transform* pTr_Bullet = mEntity->GetComponent<Transform>();

			pTr_Bullet->mPos = { pBody_gun->mPos.x + gpSystemManager->mPlayerSystem->offset.x, 
							     pBody_gun->mPos.y + gpSystemManager->mPlayerSystem->offset.y, 
								 0.0f };
			
			pTr_Bullet->mRotationDir = { 0.0f, 0.0f, 1.0f };
			pTr_Bullet->mRotate = atan2f(gpSystemManager->mPlayerSystem->sinAngle,
				gpSystemManager->mPlayerSystem->cosAngle) * 180.0f / PI;

			pBody_Bullet->mVel = { 35.0f * gpSystemManager->mPlayerSystem->cosAngle,
								   35.0f * gpSystemManager->mPlayerSystem->sinAngle };//IS ADDING VELOCITY TO THE BULLET BASED ON PLAYER FACING ARMS.

			
			pBody_Bullet->mPos = pBody_gun->mPos + gpSystemManager->mPlayerSystem->offset;

			for (auto c : mEntity->mComponents)
				c->Init();

			Entity* pFBPtcl = gpObjectFactory->CreateRuntimeObject("ParticleFireBullet.json");

			Transform* pPtTr = pFBPtcl->GetComponent<Transform>();
			Particle* pPtPtcl = pFBPtcl->GetComponent<Particle>();

			pPtTr->mPos = pTr_Bullet->mPos;
			pPtPtcl->position_adjust *= glm::normalize(glm::vec3(pBody_Bullet->mVel, 0.0f));
			glm::vec2 velNorm = glm::normalize(pBody_Bullet->mVel);
			pPtPtcl->velocity = glm::vec3(velNorm.x, velNorm.y, 0);
			gpAudioManager->PlaySFX("FireGun.mp3", 0.6f, 0.9f, -10.0f, 10.0f);
		}
	}

	//if (input->isTriggered(E)) {
	if (!gpSystemManager->mPlayerSystem->isFire ) {
		if (gpSystemManager->mPlayerSystem->mFireBody) {

			float dis = glm::distance(gpSystemManager->mPlayerSystem->mFireBody->mPos, gpSystemManager->mPlayerSystem->pBody->mPos);

			if (dis <= 2.0f) {
				gpAudioManager->Play("PlayerGetsGun.mp3");
				gpSystemManager->mPlayerSystem->isFire = true;
			}

			gpAudioManager->Play("DiscoveringAKey.wav");
		}
	}


		/*
		if (!gpSystemManager->mPlayerSystem->isIce) {
			float dis = glm::distance(gpSystemManager->mPlayerSystem->mFireBody->mPos,
				gpSystemManager->mPlayerSystem->pBody->mPos
			);

			if (dis <= 2.0f) {
				gpSystemManager->mPlayerSystem->isFire = true;
			}
		}*/
	//}

	return this;
}

void PlayerState::Enter() 
{
	if (playerBody == nullptr)
	{
		playerBody = gpSystemManager->mPlayerSystem->GetPlayerEntity()->GetComponent<Body>();
		
	}
	pAnim = gpSystemManager->mPlayerSystem->GetPlayerEntity()->GetComponent<Animation>();

}

void PlayerState::Update() {
	
		playerBody->mVel.x = glm::clamp(playerBody->mVel.x, -VELOCITY_CLAMP, VELOCITY_CLAMP);
		//playerBody->mVel.y = glm::clamp(playerBody->mVel.y, -300.0f, 35.0f);
		
		#ifdef _DEBUG
		{
			ImGui::Begin("Physics");                         

			ImGui::SliderFloat("runForce", &RUN_FORCE, 100.0f, 2000.0f);
			ImGui::SliderFloat("jumpForce", &JUMP_FORCE, 1000.0f, 100000.0f);
			ImGui::SliderFloat("airForce", &AIR_CONTROL_FORCE, 100.0f, 3000.0f);
			ImGui::SliderFloat("runSpeed", &RUN_SPEED, 20.0f, 100.0f);
			ImGui::SliderFloat("walkSpeed", &WALK_SPEED, 20.0f, 100.0f);
			ImGui::SliderFloat("grabWalkSpeed", &GRAB_WALK_SPEED, 1.0f, 20.0f);
			ImGui::SliderFloat("jumpHoldTime", &JUMP_HOLD_TIME, 0.0f, 1.0f);
			ImGui::SliderFloat("extraJumpForce", &EXTRA_JUMP, 500.0f, 5000.0f);
			ImGui::Text("playerVelocity = %f %f", playerBody->mVel.x, playerBody->mVel.y);
			ImGui::Text("playerPosition = %f %f", playerBody->mPos.x, playerBody->mPos.y);
			ImGui::Text("mTotalForce in X dir = %f", playerBody->mTotalForce.x);
			ImGui::Checkbox("isGrounded", &gpSystemManager->mPlayerSystem->isGrounded);
			ImGui::Checkbox("jumpingUp", &gpSystemManager->mPlayerSystem->jumpState.jumpingUp);
			ImGui::Checkbox("jumpingDown", &gpSystemManager->mPlayerSystem->jumpState.jumpingDown);

			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		#endif
	}

PlayerState* IdleState::HandleInput(InputManager* input) 
{

	/*
	if (input->isTriggered(LEFT))
	{
		if (pAnim != nullptr) 
 		{
       			pAnim->mAnimation_Stack.push_back(FORWARD_ANIMATION);
				
		}
	}
	if (input->isTriggered(RIGHT))
	{
		if (pAnim != nullptr)
		{
			pAnim->mAnimation_Stack.push_back(FORWARD_ANIMATION);
		}
	}
	*/

	// if starts falling in idle state send to falling state
	if (playerBody->mVel.y < fallStateEnterValue)
		return &gpSystemManager->mPlayerSystem->fallState;

	// goes to run state if right or left is pressed
	if (input->isPressed(RIGHT) || input->isPressed(D)) { 
		// start turning right
		gpSystemManager->mPlayerSystem->runState.isRunningRight = true;
		gpSystemManager->mPlayerSystem->runState.isRunningLeft = false;
		return &gpSystemManager->mPlayerSystem->runState;
	}
	else if (input->isPressed(LEFT) || input->isPressed(A)) {
		gpSystemManager->mPlayerSystem->runState.isRunningLeft = true;
		gpSystemManager->mPlayerSystem->runState.isRunningRight = false;
		return &gpSystemManager->mPlayerSystem->runState;
	}
	else if (input->isTriggered(JUMP))
	{
		/*
		if (pAnim != nullptr)
		{
			pAnim->mAnimation_Stack.push_back(JUMP_ANIMATION);
		}*/
		//Add jump sound here
		return &gpSystemManager->mPlayerSystem->jumpState;
	}

	// grab mechanic
	/*if (input->isTriggered(LCTRL) && gpSystemManager->mPlayerSystem->grabbingState.grabbedBody) {
		return &gpSystemManager->mPlayerSystem->grabbingState;
	}*/

	
	PlayerState::HandleInput(input);

	return nullptr;
}

void IdleState::Enter() {
	PlayerState::Enter();
	//std::cout << "Entered Idle State" << std::endl;
}

void IdleState::Update() {
	PlayerState::Update();
	//std::cout << "Entered Idle State" << std::endl;
}

PlayerState* RunningState::HandleInput(InputManager* input) {
	
	/*
	if (input->isTriggered(LEFT))
	{
		if (pAnim != nullptr)
		{
			pAnim->mAnimation_Stack.push_back(FORWARD_ANIMATION);
		}
	}
	if (input->isTriggered(RIGHT))
	{
		if (pAnim != nullptr)
		{
			pAnim->mAnimation_Stack.push_back(FORWARD_ANIMATION);
		}
	}
	*/

	if (input->isTriggered(JUMP)) 
	{
		return &gpSystemManager->mPlayerSystem->jumpState;
	}

	// if starts falling in running state send to falling state
	if(playerBody->mVel.y < fallStateEnterValue)
		return &gpSystemManager->mPlayerSystem->fallState;

	if (input->isPressed(RIGHT) || input->isPressed(D)) {
		forceX = RUN_FORCE;
	}
	if((input->isReleased(RIGHT) && input->isReleased(D)) && isRunningRight)
		return &gpSystemManager->mPlayerSystem->idleState;
	if((input->isReleased(LEFT) && input->isReleased(A)) && isRunningLeft)
		return &gpSystemManager->mPlayerSystem->idleState;

	if (input->isPressed(LEFT) || input->isPressed(A)) {
		forceX = -RUN_FORCE;
	}
	/*if (input->isTriggered(LCTRL) && gpSystemManager->mPlayerSystem->grabbingState.grabbedBody) {
		return &gpSystemManager->mPlayerSystem->grabbingState;
	}*/
	if (input->isPressed(LSHIFT)) {
		forceX *= 1.5f;
		VELOCITY_CLAMP = RUN_SPEED;
	}
	else {
		VELOCITY_CLAMP = WALK_SPEED;
	}


	
	PlayerState::HandleInput(input);

	return nullptr;
}

void RunningState::Enter() {
	PlayerState::Enter();
	
	// set Running Animation
	//std::cout << "Entered Running State" << std::endl;

}

void RunningState::Update() {
	
	// update the body component
	playerBody->mTotalForce.x += forceX;

	PlayerState::Update();
}

PlayerState* JumpingState::HandleInput(InputManager* input) {
	
	if (gpSystemManager->mPlayerSystem->isGrounded && !jumpingUp) {
		forceAirX = 0.0f;
		return &gpSystemManager->mPlayerSystem->idleState;
	}

	if (input->isPressed(RIGHT) || input->isPressed(D)) {
 		forceAirX = AIR_CONTROL_FORCE;
	} else if (input->isPressed(LEFT) || input->isPressed(A)) {
		forceAirX = -AIR_CONTROL_FORCE;
	}
	else {
		forceAirX = 0.0f;
	}
	if (input->isPressed(JUMP)) {
		isJumpPressed = true;
	}
	if (input->isReleased(JUMP)) {
		isJumpPressed = false;
	}
	
	PlayerState::HandleInput(input);

	return nullptr;
}

JumpingState::JumpingState() : jumpingUp(false), jumpingDown(false), forceAirX(0.0f) {

}

void JumpingState::Enter() {
	forceY = JUMP_FORCE;
	jumpTime = JUMP_HOLD_TIME;
	PlayerState::Enter();
	playerBody->applyFriction = false;
	gpAudioManager->PlaySFX("PlayerJump.mp3", 0.5f, 0.7f, -5.0f, 5.0f);
	// set Jumping Animation
	//std::cout << "Entered Jumping State" << std::endl;
}

void JumpingState::Exit() {
	jumpingUp = false;
	jumpingDown = false;
	playerBody->applyFriction = true;
	gpAudioManager->PlaySFX("PlayerTouchesGround.mp3", 0.5f, 0.7f, -5.0f, 5.0f);

}

void FallingState::Exit()
{
	isFalling = false;
	playerBody->applyFriction = true;
	gpAudioManager->PlaySFX("PlayerTouchesGround.mp3", 0.5f, 0.7f, -5.0f, 5.0f);
}

void JumpingState:: Update() {
	if (gpSystemManager->mPlayerSystem->isGrounded && !jumpingUp) {
		// update the body component
		playerBody->mTotalForce.y = forceY;
		jumpingUp = true;
		jumpingDown = false;
	}
	else {
		if (isJumpPressed && jumpTime > 0) {
			playerBody->mTotalForce.y = EXTRA_JUMP;
			
			jumpTime -= (float)gpFrameRateController->GetFrameTime();
		}
		// air control
		playerBody->mTotalForce.x += forceAirX;
		if (jumpingUp && playerBody->mVel.y < -0.01f) {
			jumpingDown = true;
			jumpingUp = false;
		}

		if (jumpingDown) {
			playerBody->mTotalForce.y += FALLING_FORCE;
		}
	}


	PlayerState::Update();
}

GrabbingState::GrabbingState() : pushingForce(0.0f) {}

void GrabbingState::Update() {
	// update the body component
	playerBody->mTotalForce.x += pushingForce;
	if (grabbedBody) {

		if (abs(playerBody->mVel.x) > 1.0f && !isPushing) {
			isPushing = true;
			// play sound here

			GrabbingChannel = gpAudioManager->Play("PlayerDragging.mp3", -1);

		}

		grabbedBody->mTotalForce.x += pushingForce * (playerBody->mInvMass * grabbedBody->mMass);
		grabbedBody->mVel.x = glm::clamp(grabbedBody->mVel.x, -playerBody->mVel.x, playerBody->mVel.x);

		grabbedBody = nullptr;
	}
	PlayerState::Update();
}

void GrabbingState::Enter() {
	isPushing = false;

	PlayerState::Enter();
	VELOCITY_CLAMP = WALK_SPEED;
}

void GrabbingState::Exit() {
	isPushing = false;
	gpAudioManager->Stop(GrabbingChannel);
	VELOCITY_CLAMP = WALK_SPEED;
	grabbedBody = nullptr;
}

// No longer in use
PlayerState* GrabbingState::HandleInput(InputManager* input) {
	if (input->isReleased(LCTRL)) {
		return &gpSystemManager->mPlayerSystem->idleState;
	}
	if (input->isPressed(RIGHT) || input->isPressed(D)) {
		
		pushingForce = RUN_FORCE;
	} else if (input->isPressed(LEFT) || input->isPressed(A)) {
		pushingForce = -RUN_FORCE;
	}
	else {
		pushingForce = 0.0f;
	}

	return nullptr;
}

FallingState::FallingState(): isFalling(false) {}

void FallingState::Enter()
{
	PlayerState::Enter();

	gpSystemManager->mPlayerSystem->isGrounded = false;
	isFalling = true;
	playerBody->applyFriction = false;
}

void FallingState::Update()
{
	// air control
	playerBody->mTotalForce.x += forceAirX;
	playerBody->mTotalForce.y += FALLING_FORCE;

	PlayerState::Update();
}

PlayerState * FallingState::HandleInput(InputManager * input)
{
	if (gpSystemManager->mPlayerSystem->isGrounded) {
		forceAirX = 0.0f;
		return &gpSystemManager->mPlayerSystem->idleState;
	}

	if (input->isPressed(RIGHT) || input->isPressed(D)) {
		forceAirX = AIR_CONTROL_FORCE;
	}
	else if (input->isPressed(LEFT) || input->isPressed(A)) {
		forceAirX = -AIR_CONTROL_FORCE;
	}

	PlayerState::HandleInput(input);

	return nullptr;
}


