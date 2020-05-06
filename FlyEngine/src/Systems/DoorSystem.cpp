/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Ivan Cazares, ivan.c, 60004418
- End Header --------------------------------------------------------*/


#include "../pch.h"
#include "DoorSystem.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/AudioManager.h"

extern ComponentManager* gpComponentManager;
extern AudioManager * gpAudioManager;

DoorSystem::DoorSystem()
{
}


DoorSystem::~DoorSystem()
{
}

void DoorSystem::Update() {
	Component* c = gpComponentManager->mDoor->GetHead();
	while (nullptr != c) {
		Door* pDoor = static_cast<Door*>(c);
		Body* pBody = pDoor->mpOwner->GetComponent<Body>();

		if (pDoor->mDoorType == PressOnce) {
			if (!pDoor->isOpen) {
				if (pDoor->mKeyHit) {
					pDoor->tempDoorLength += 0.1f;
					pBody->mPos.y += 0.1f;
					pBody->pTr->mDirtyFlag = true;
					if (pDoor->tempDoorLength >= pDoor->doorLength) {
						pDoor->isOpen = true;
						
					}
				}
			}
			pDoor->mPrevKeyHit  =  pDoor->mKeyHit;

			if (pDoor->isClose) {
				if (pDoor->closeDoor)
				{
					pDoor->tempDoorLength -= 0.1f;
					pBody->mPos.y -= 0.1f;
					pBody->pTr->mDirtyFlag = true;
					if (pDoor->tempDoorLength <= 0.0f) {
						pDoor->closeDoor = false;
						pDoor->isClose = false;
					}
				}
			}
		}

		else if (pDoor->mDoorType == PressConstantly) {
			if (pDoor->mKeyHit == true) {
				if (!pDoor->isOpen) {
					pDoor->isClose = false;
					pDoor->tempDoorLength += 0.1f;
					pBody->mPos.y += 0.1f;
					pBody->pTr->mDirtyFlag = true;
					if (pDoor->tempDoorLength >= pDoor->doorLength)
						pDoor->isOpen = true;
				}
				pDoor->mPrevKeyHit = pDoor->mKeyHit;
				pDoor->mKeyHit = false;
			}
			else {
				if (!pDoor->isClose) {
					pDoor->tempDoorLength -= 0.1f;
					pBody->mPos.y -= 0.1f;
					pBody->pTr->mDirtyFlag = true;
					pDoor->isOpen = false;
					if (pDoor->tempDoorLength <= 0.0f) {
						pDoor->isClose = true;
					}
				}
				pDoor->mPrevKeyHit = pDoor->mKeyHit;
			}
		}

		else if (pDoor->mDoorType == PressMultipleTimes) {
			if (pDoor->mKeyForMultipleHit) {
				if (!pDoor->isOpen) {
					pDoor->isClose = false;
					pDoor->tempDoorLength += 0.1f;
					pBody->mPos.y += 0.1f;
					pBody->pTr->mDirtyFlag = true;
					if (pDoor->tempDoorLength >= pDoor->doorLength)
						pDoor->isOpen = true;
				}
			}
			else {
				if (!pDoor->isClose) {
					pDoor->tempDoorLength -= 0.1f;
					pBody->mPos.y -= 0.1f;
					pBody->pTr->mDirtyFlag = true;
					pDoor->isOpen = false;
					if (pDoor->tempDoorLength <= 0.0f) {
						pDoor->isClose = true;
					}
				}
			}
			
		}

		c = c->GetNext();
	}
}