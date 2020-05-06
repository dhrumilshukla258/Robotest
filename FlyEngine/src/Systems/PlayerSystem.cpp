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
#include "HealthSystem.h"

#include "../Systems/ControlSystem.h"
#include "../Managers/CameraManager.h"
#include "../Managers/SceneManager.h"
extern CameraManager* gpCameraManager;
extern SceneManager* gpSceneManager;

#define PI 3.14159265358f

extern InputManager* gpInputManager;
extern AudioManager * gpAudioManager;
extern SystemManager * gpSystemManager;
extern bool gameStarted;
PlayerSystem::PlayerSystem()
{
	hasDied = false;
	isGrounded = true;
	// we start in the idle state
	_state = &idleState;
	playerDirection = East;
	tweenL = tweeny::from(0.0f).to(1.0f).during(100.0f);
	tweenR = tweeny::from(-180.0f).to(0.0f).during(100.0f);
	tweenR.seek(1); // assuming starting direction to be east
}

PlayerSystem::~PlayerSystem()
{
}

void PlayerSystem::SetPlayerEntity(Entity* en)
{
	isFire = false;
	hasDied = false;
	gpSystemManager->mHealthSystem->respawnTime = 1.5f;
	gpSystemManager->mHealthSystem->timeCounter = 0.0f;
	gpSystemManager->mHealthSystem->diedRecently = false;
	gpSystemManager->mHealthSystem->immortalityTime = 0.5f;

	playerEntity = en;
	if (nullptr != en) {
		pBody = en->GetComponent<Body>();
		idleState.playerBody = pBody;
		runState.playerBody = pBody;
		jumpState.playerBody = pBody;
		grabbingState.playerBody = pBody;
		fallState.playerBody = pBody;

		idleState.pAnim = en->GetComponent<Animation>();
		runState.pAnim = en->GetComponent<Animation>();
		jumpState.pAnim = en->GetComponent<Animation>();
		grabbingState.pAnim = en->GetComponent<Animation>();
		fallState.pAnim = en->GetComponent<Animation>();
	}
}


void PlayerSystem::ShowDeathAnimation()
{
	if (!hasDied) {
	
		_state->pAnim->mAnimation_Stack.push_back(DEATH_ANIMATION);
		gpAudioManager->Play("PlayerDie3.mp3");
		hasDied = true;
	}
}

void PlayerSystem::ShowRebirthAnimation()
{
	_state->pAnim->mAnimation_Stack.push_back(RESPAWN_ANIMATION);
}


void PlayerSystem::SetFireGun(Entity* en) {
	fireGun = en;
	Transform* pTr_fGun = en->GetComponent<Transform>();
	Body* pBody_fGun = en->GetComponent<Body>();

	mFireTransform = pTr_fGun;
	mFireScale = pTr_fGun->mScale;
	mFireBody = pBody_fGun;
}

void PlayerSystem::SetIceGun(Entity* en) {
	iceGun = en;

	Transform* pTr_iGun = en->GetComponent<Transform>();
	Body* pBody_iGun = en->GetComponent<Body>();

	mIceTransform = pTr_iGun;
	mIceScale = pTr_iGun->mScale;
	mIceBody = pBody_iGun;
}

void PlayerSystem::AddCheckpoint(Entity* _pEntity)
{
	Transform* pTr = _pEntity->GetComponent<Transform>();
	glm::vec3 mNewCheckpoint = pTr->mPos;
	
	if (!(mCpVec.empty())) {
		int size = mCpVec.size();
		for (int i = 0; i < size; ++i) {
			if (mCpVec[i].x < mNewCheckpoint.x) {
				//do nothing
			}
			else {
				// if vector full, resize
				if (mCpVec.capacity() == mCpVec.size()) {
					mCpVec.reserve(mCpVec.capacity() + 10);
				}

				//else push
				mCpVec.push_back(mNewCheckpoint);

				// reorder
				for (int j = mCpVec.size()-2; j >= i && j>=0; --j) {
					mCpVec[j + 1] = mCpVec[j];
				}
				mCpVec[i] = mNewCheckpoint;
				return;
			}
		}
	}

	// if vector full, resize
	if (mCpVec.capacity() == mCpVec.size()) {
		mCpVec.reserve(mCpVec.capacity() + 10);
	}

	//else push
	mCpVec.push_back(mNewCheckpoint);
}

Entity* PlayerSystem::GetPlayerEntity()
{
	return playerEntity;
}

Entity* PlayerSystem::GetFireGun() 
{
	return fireGun;
}

Entity* PlayerSystem::GetIceGun()
{
	return iceGun;
}

Directions currentDir = East;
float PlayerSystem::FlipPlayer(Directions dir) {
	if (dir == currentDir) {
		if (dir == West) {
			
			pBody->pTr->mDirtyFlag = true;
			return -180.0f;

			/*if (tweenL.progress() < 1) {
				pBody->pTr->mDirtyFlag = true;
				mFireTransform->mScale.z = -fabs(mFireTransform->mScale.z);
				return tweenL.step(0.05f);
			}
			else
				return -180.0f;
				*/
		}
		else
		{
			pBody->pTr->mDirtyFlag = true;
			return 0.0f;
			/*
			if (tweenR.progress() < 1) {
				pBody->pTr->mDirtyFlag = true;
				mFireTransform->mScale.z = fabs(mFireTransform->mScale.z);
				return tweenR.step(0.05f);
			}
			else
				return 0.0f;
				*/
		}

	}
	else {
		if (dir == East) {
			currentDir = East;
			//tweenR.seek(0);
			return -180.0f;
		}
		else {
			currentDir = West;
			//tweenL.seek(0);
			return 0.0f;
		}
	}
	return 0.0f;
}

void PlayerSystem::HandleInput() {

	PlayerState* newState = _state->HandleInput(gpInputManager);
	if (newState != nullptr) {
		_state->Exit();
		_state = newState;
		_state->Enter();
	}
}

//glm::vec2 deathPos, respawnPos;
//bool isRespawn = false;
void PlayerSystem::ResetToPosition(glm::vec2 pos) {
	//deathPos = pBody->mPos;
	//respawnPos = pos;
	pBody->mPos = pos;
	pBody->mVel = glm::vec2(0.0f);
	pBody->mTotalForce = glm::vec2(0.0f);
	//isRespawn = true;
	hasDied = false;
	playerDirection = East;

	isGrounded = true;
	_state->Exit();
	_state = &idleState;
	_state->Enter();
}

void PlayerSystem::UpdatePlayerAimDirection() {
	InputManager* input = gpInputManager;
	int dir = 0;
	float pi = (float)acos(-1);
	if (input->isPressed(UP) || input->isPressed(W)) {
		dir += Directions::North;
	}
	if (input->isPressed(DOWN) || input->isPressed(S)) {
		dir += Directions::South;
	}
	if (input->isPressed(LEFT) || input->isPressed(A)) {
		dir += Directions::West;
		playerDirection = Directions::West;
	}
	if (input->isPressed(RIGHT) || input->isPressed(D)) {
		dir += Directions::East;
		playerDirection = Directions::East;
	}

	// checking if the calculated direction was valid
	if (isFire) {
		//mFireTransform->mScale = mFireScale;
		mFireTransform->mRotationDir = { 0.0f, 0.0f, 1.0f };
		mFireTransform->mDirtyFlag = true;
		mFireTransform->mPos = playerEntity->GetComponent<Transform>()->mPos;
		mFireBody->mPos = pBody->mPos;
	}

	aimDirection = static_cast<Directions>(dir);
	switch (aimDirection) {
	case Directions::North:
		cosAngle = 0.0f;
		sinAngle = 1.0f;
		if (isFire) {
			mFireTransform->mRotate = 90.0f;
			if (pBody->pTr->mRotate == 0.0f) {
				offset = { -0.35f, 1.0f };
				mFireTransform->mPos.x -= 0.0f;
				mFireTransform->mPos.y += 1.6f;
				mFireBody->mPos.x -= 0.0f;
				mFireBody->mPos.y += 1.6f;
			}
			else {
				offset = { 0.35f, 1.0f };
				mFireTransform->mPos.x += 0.0f;
				mFireTransform->mPos.y += 1.6f;
				mFireBody->mPos.x += 0.0f;
				mFireBody->mPos.y += 1.6f;
			}
			
		}
		break;
	//case Directions::South:
	//	cosAngle = 0.0f;
	//	sinAngle = -1.0f;
	//	if (isFire) {
	//		mFireTransform->mRotate = 270.0f;
	//		if (pBody->pTr->mRotate == 0.0f) {
	//			mFireTransform->mPos.x -= 0.9f;
	//			mFireTransform->mPos.y += 1.6f;
	//			mFireBody->mPos.x -= 0.9f;
	//			mFireBody->mPos.y += 1.6f;
	//		}
	//		else {
	//			mFireTransform->mPos.x += 1.0f;
	//			mFireTransform->mPos.y += 1.6f;
	//			mFireBody->mPos.x += 1.0f;
	//			mFireBody->mPos.y += 1.6f;
	//		}
	//	}
	//	break;
	case Directions::East:
		cosAngle = 1.0f;
		sinAngle = 0.0f;
		offset = { 1.0f, 0.4f };
		if (isFire) {
			mFireTransform->mRotate = 0.0f;
			mFireTransform->mPos.x += 1.0f;
			mFireTransform->mPos.y += 0.4f;
			mFireBody->mPos.x += 1.0f;
			mFireBody->mPos.y += 0.4f;
		}
		break;
	case Directions::West:
		cosAngle = -1.0f;
		sinAngle = 0.0f;
		offset = { -1.0f, 0.4f };
		if (isFire) {
			mFireTransform->mRotate = 180.0f;
			mFireTransform->mPos.x -= 1.0f;
			mFireTransform->mPos.y += 0.4f;
			mFireBody->mPos.x -= 1.0f;
			mFireBody->mPos.y += 0.4f;
		}
		break;
	case Directions::NorthEast:
		cosAngle = 1.0f/ 1.41421356237f;
		sinAngle = 1.0f/ 1.41421356237f;
		offset = { 0.183f, 0.8f };
		if (isFire) {
			mFireTransform->mRotate = 45.0f;
			//mFireTransform->mPos.x += 1.5f;
			mFireTransform->mPos.y += 1.2f;
			//mFireBody->mPos.x += 1.5f;
			mFireBody->mPos.y += 1.2f;
		}
		break;
	case Directions::NorthWest:
		cosAngle = -1.0f / 1.41421356237f;
		sinAngle =  1.0f / 1.41421356237f;
		offset = { -0.48f, 0.995f };
		if (isFire) {
			mFireTransform->mRotate = 135.0f;
			mFireTransform->mPos.x += 0.1f;
			mFireTransform->mPos.y += 1.2f;
			mFireBody->mPos.x += 0.1f;
			mFireBody->mPos.y += 1.2f;
		}
		break;
	case Directions::SouthEast:
		cosAngle =  1.0f / 1.41421356237f;
		sinAngle = -1.0f / 1.41421356237f;
		if (isFire) {
			mFireTransform->mRotate = 315.0f;
			mFireTransform->mPos.x += 0.2f;
			mFireTransform->mPos.y -= 0.5f;
			mFireBody->mPos.x += 0.2f;
			mFireBody->mPos.y -= 0.5f;
		}
		break;
	case Directions::SouthWest:
		cosAngle = -1.0f / 1.41421356237f;
		sinAngle = -1.0f / 1.41421356237f;
		if (isFire) {
			mFireTransform->mRotate = 225.0f;
			//mFireTransform->mPos.x += 0.2f;
			mFireTransform->mPos.y -= 0.5f;
			//mFireBody->mPos.x += 0.2f;
			mFireBody->mPos.y -= 0.5f;
		}
		break;
	default:
		if(playerDirection == East) {
			cosAngle = 1.0f;
			sinAngle = 0.0f;
			offset = { 1.0f, 0.4f };
			if (isFire) {
				mFireTransform->mRotate = 0.0f;
				mFireTransform->mPos.x += 1.0f;
				mFireTransform->mPos.y += 0.4f;
				mFireBody->mPos.x += 1.0f;
				mFireBody->mPos.y += 0.4f;
			}
		}
		else {
			cosAngle = -1.0f;
			sinAngle = 0.0f;
			offset = { -1.0f, 0.4f };
			if (isFire) {
				mFireTransform->mRotate = 180.0f;
				mFireTransform->mPos.x -= 1.0f;
				mFireTransform->mPos.y += 0.4f;
				mFireBody->mPos.x -= 1.0f;
				mFireBody->mPos.y += 0.4f;
			}
		}
	}
}

float health_save = 0.0f;
bool god_mode = false;
void PlayerSystem::Update(float _frameTime)
{

	if (!hasDied) {
		if (nullptr != playerEntity) {

			//particle enabled disabled
			Particle* pPart;
			pPart = playerEntity->GetComponent<Particle>();
			if (pPart != nullptr)
			{
				if (isGrounded)
				{
					pPart->particle_enable = true;
				}
				else
				{
					pPart->particle_enable = false;
				}
			}

			// God mode
			if (gpInputManager->isTriggered(F1)) {
				if (playerEntity) {
					Health* pHealth = playerEntity->GetComponent<Health>();
					if (pHealth) {
						if (!god_mode) {
							health_save = pHealth->mHealthLeft;
							pHealth->mHealthLeft = std::numeric_limits<float>::infinity();
						}
						else {
							pHealth->mHealthLeft = health_save;
						}
						god_mode = !god_mode;
					}
				}
			}

			if (gpInputManager->isTriggered(J)) {
				gameStarted = false;
				gpSceneManager->setNextScene("WinScreen.json");
				gpCameraManager->SnapToPosition(glm::vec3(40.0f, 9.0f, 150.0f));
				gpSystemManager->mControlSystem->selectedItem = Playagain;
				gpSystemManager->mControlSystem->menuStartingIndex = Playagain;
				gpSystemManager->mControlSystem->menuLength = 2;
				//gpCameraManager->mPosition = glm::vec3(38.121f, 12.486f, 146.66f);

				// I TRIED!
				/*gpCameraManager->ResetCamera();
				gpCameraManager->mYaw = -89.8f;
				gpCameraManager->mPitch = -4.2f;
				gpCameraManager->SnapToPosition(glm::vec3(38.121f, 12.486f, 146.66f));*/
			}

			//Player lose
			if (gpInputManager->isTriggered(I)) {
				Health * pHealth = playerEntity->GetComponent<Health>();
				pHealth->mHealthLeft = -1.0f;
			}

			// Checkpoint cheat forward
			if (gpInputManager->isTriggered(L)) {
				auto itr = std::find(mCpVec.begin(), mCpVec.end(), mCheckpoint);
				if (itr != mCpVec.end()) {
					if ((itr + 1) != mCpVec.end()) {
						Transform* pPlTr = playerEntity->GetComponent<Transform>();
						if (pPlTr) {
							mCheckpoint = *(itr + 1);
							ResetToPosition(glm::vec2(mCheckpoint));
							gpCameraManager->ResetCamera();
						}
					}
				}
			}

			// Checkpoint cheat backward
			if (gpInputManager->isTriggered(K)) {
				auto itr = std::find(mCpVec.begin(), mCpVec.end(), mCheckpoint);
				if (itr != mCpVec.begin()) {
					Transform* pPlTr = playerEntity->GetComponent<Transform>();
					if (pPlTr) {
						mCheckpoint = *(itr - 1);
						ResetToPosition(glm::vec2(mCheckpoint));
						gpCameraManager->ResetCamera();
					}
				}
			}

			UpdatePlayerAimDirection();

			if (mBulletTime > 0.0f)
				mBulletTime -= _frameTime;

			if (pBody->pTr) {
				pBody->pTr->mRotate = FlipPlayer(playerDirection);

				if (isFire) {
					if (pBody->pTr->mRotate == 0.0f)
						mFireTransform->mScale.z = fabs(mFireTransform->mScale.z);
					else
						mFireTransform->mScale.z = -fabs(mFireTransform->mScale.z);
				}

			}

			// handle new input before updating
			HandleInput();

			// then update state
			_state->Update();
		}
	}
}