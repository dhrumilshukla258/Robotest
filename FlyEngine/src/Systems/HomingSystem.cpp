/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "HomingSystem.h"

#include "../Managers/ComponentManager.h"
#include "../Managers/SystemManager.h"
#include "../Systems/PlayerSystem.h"

#define PI 3.14159265358f
#define stopHoming 0.5f

HomingSystem::HomingSystem()
{
}


HomingSystem::~HomingSystem()
{
}

void HomingSystem::Update(float _frameTime)
{
	Component* c = gpComponentManager->mHoming->GetHead();

	if (nullptr == c) return;
	Entity* playerEntity = gpSystemManager->mPlayerSystem->GetPlayerEntity();

	if (nullptr != playerEntity) {
		Transform* playerTransform = playerEntity->GetComponent<Transform>();
		while (nullptr != c) {
			Homing* pHom = static_cast<Homing*>(c);
			if (pHom->mHomingActive) {
				if (pHom->mInitialPadding >= 0.0f) {
					pHom->mInitialPadding -= _frameTime;
				}
				else {
					Body* pBody = pHom->mpOwner->GetComponent<Body>();
					Transform* pTr = pHom->mpOwner->GetComponent<Transform>();

					float mainAngle = pTr->mRotate * PI / 180.0f;

					glm::vec2 targetHoming, rotateHoming;

					rotateHoming = { pBody->mVel.x*cosf(PI / 2) - pBody->mVel.y*sinf(PI / 2),
										pBody->mVel.x*sinf(PI / 2) + pBody->mVel.y*cosf(PI / 2) };



					//Right now homing has just one target and thats player
					pHom->mTarget = { playerTransform->mPos.x, playerTransform->mPos.y };
					targetHoming = pHom->mTarget - pBody->mPos;


					//Getting +ve,-ve Angle
					float angle = glm::dot(rotateHoming, targetHoming);

					if (angle >= 0)
						mainAngle += pHom->mRotSpeed * _frameTime;
					else
						mainAngle -= pHom->mRotSpeed * _frameTime;

					if (mainAngle >= PI)
						mainAngle -= PI;
					else if (mainAngle <= -PI)
						mainAngle += PI;

					pBody->mVel = { pHom->mSpeed * cosf(mainAngle), pHom->mSpeed * sinf(mainAngle) };

					pTr->mRotate = mainAngle * 180.0f / PI;

					if (!pHom->mKeepHoming) {
						pHom->mHomingActive = false;
					}
				}

			}
			else {
				if (!pHom->mKeepHoming) {
					Body* pBody = pHom->mpOwner->GetComponent<Body>();
					if (pBody->mVel.x >= 0.0f) {
						if (pBody->mVel.x <= stopHoming)
							pBody->mVel.x = 0.0f;
						else
							pBody->mVel.x -= stopHoming;
					}
					else if (pBody->mVel.x <= 0.0f) {
						if (pBody->mVel.x >= -stopHoming)
							pBody->mVel.x = 0.0f;
						else
							pBody->mVel.x += stopHoming;
					}
					
					if (pBody->mVel.y >= 0.0f) {
						if (pBody->mVel.y <= stopHoming)
							pBody->mVel.y = 0.0f;
						else			
							pBody->mVel.y -= stopHoming;
					}
					else if (pBody->mVel.y <= 0.0f) {
						if (pBody->mVel.y >= -stopHoming)
							pBody->mVel.y = 0.0f;
						else			
							pBody->mVel.y += stopHoming;
					}
				}
			}
			c = c->GetNext();
		}
	}
}