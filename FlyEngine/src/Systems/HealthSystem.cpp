/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/


#include "../pch.h"
#include "HealthSystem.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/AudioManager.h"
#include "../Managers/ObjectFactory.h"
#include "../Managers/SceneManager.h"
#include "../Managers/SystemManager.h"
#include "../Managers/CameraManager.h"
#include "../Systems/PlayerSystem.h"
#include "../Systems/ControlSystem.h"

extern ComponentManager* gpComponentManager;
extern AudioManager* gpAudioManager;
extern ObjectFactory* gpObjectFactory;
extern SystemManager* gpSystemManager;
extern CameraManager* gpCameraManager;
extern SceneManager* gpSceneManager;

extern bool gameStarted;

#define FALL_DEATH_VELOCITY -200.0f

HealthSystem::HealthSystem()
{
	
}


HealthSystem::~HealthSystem()
{
}



void HealthSystem::Update()
{
	Component* c = gpComponentManager->mHealth->GetHead();
	while (nullptr != c) {
		Health* pH = static_cast<Health*>(c);
		c = c->GetNext();
		if (PLAYER == pH->mpOwner->mType) {
			Body* player_pBody = gpSystemManager->mPlayerSystem->GetPlayerEntity()->GetComponent<Body>();
			if (player_pBody->mVel.y < FALL_DEATH_VELOCITY) {
				pH->mHealthLeft = 0.0f;
			}
			if (diedRecently) {
				timeCounter += 0.016f;
				if (timeCounter < immortalityTime) {
					continue;
				}
				else {
					diedRecently = false;
					pH->mHealthLeft = 100.0f;
					timeCounter = 0.0f;
				}
			}
		}
		
		//Write Code Here Arnold
		if (BOSS == pH->mpOwner->mType) {
			Boss* pBoss = pH->mpOwner->GetComponent<Boss>();
			if (fabs(pH->mHealthLeft - 50.0f)<1.0f && pBoss->killedAllLauncher) {//TODO Not allways is 50 sometimes float 49.99999 modify 
				
				pBoss->playBackwardAnimation = true;
				pBoss->killedAllLauncher = false;
			}
		}		

		if (pH->mHealthLeft <= 0.0f) {
			if (BOSS == pH->mpOwner->mType) {
				/*gpSceneManager->setNextScene("WinScreen.json");
				gameStarted = false;
				gpSystemManager->mControlSystem->selectedItem = Playagain;
				gpSystemManager->mControlSystem->menuStartingIndex = Playagain;
				gpSystemManager->mControlSystem->menuLength = 2;*/
				gpAudioManager->StopAll();
				gpAudioManager->Play("BG06.mp3", -1);
				Boss* pBoss = pH->mpOwner->GetComponent<Boss>();
				Door* door_dr = pBoss->mDoor->GetComponent<Door>();
				door_dr->mKeyHit = true;

				if (!door_dr->mPrevKeyHit && door_dr->mKeyHit) {//press button once
					if (!door_dr->mPrevKeyHit && door_dr->mKeyHit) {//press button once
						gpAudioManager->Play("ButtonPress2.mp3");
						gpAudioManager->Play("DoorOpen.mp3");
					}
					door_dr->mKeyForMultipleHit = !door_dr->mKeyForMultipleHit;
				}
			}

			if (!diedRecently && pH->mpOwner->mType == PLAYER) {
				Body* player_pBody = gpSystemManager->mPlayerSystem->GetPlayerEntity()->GetComponent<Body>();
				player_pBody->mVel = glm::vec2(0.0f);
				gpSystemManager->mPlayerSystem->ShowDeathAnimation();
			}
			
			//Respawn from check point
			if (PLAYER == pH->mpOwner->mType && !diedRecently) {

				//PlayerDeadSuffer
				timeCounter += 0.016f;

				if (timeCounter > respawnTime) {
						timeCounter = 0.0f;
					gpAudioManager->Play("PlayerRespawns.mp3");

					Transform* player_pTr = gpSystemManager->mPlayerSystem->GetPlayerEntity()->GetComponent<Transform>();
					Body* player_pBody = gpSystemManager->mPlayerSystem->GetPlayerEntity()->GetComponent<Body>();

					player_pBody->mVel = glm::vec2(0.0f);
					player_pTr->mPos = gpSystemManager->mPlayerSystem->mCheckpoint;
					player_pTr->mDirtyFlag = true;

					pH->mHealthLeft = 100.0f;

					diedRecently = true;

					gpSystemManager->mPlayerSystem->ResetToPosition(glm::vec2(gpSystemManager->mPlayerSystem->mCheckpoint));
					gpCameraManager->ResetCamera();
					gpSystemManager->mPlayerSystem->ShowRebirthAnimation();
					}
				continue;
			}
			
			if (BURN_DOOR == pH->mpOwner->mType) {
				Entity* e = gpObjectFactory->CreateRuntimeObject("Blast_Effect.json");
				Transform* pTr = e->GetComponent<Transform>();
				pTr->mPos = pH->mpOwner->GetComponent<Transform>()->mPos;
			}

			gpObjectFactory->DeleteObject(pH->mpOwner->id);
			
		}
	}
}