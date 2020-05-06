/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "AIBehaviour.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

#include "../Managers/SystemManager.h"
extern SystemManager* gpSystemManager;
#include "../Systems/PlayerSystem.h"

#include "../Managers/ObjectFactory.h"
extern ObjectFactory* gpObjectFactory;

#include "../Managers/CollisionManager.h"
extern CollisionManager* gpCollisionManager;

extern AudioManager  * gpAudioManager;

#define PI 3.14159265358f

AIBehaviour::AIBehaviour()
{
}


AIBehaviour::~AIBehaviour()
{
}

void SetToEntity(Entity* mEntity, Transform* pTr, EnemyAI* pEA)
{
	Body* pBody_Bullet = mEntity->GetComponent<Body>();
	Transform* pTr_Bullet = mEntity->GetComponent<Transform>();

	pTr_Bullet->mPos = { pTr->mPos.x , pTr->mPos.y, pTr->mPos.z };
	pTr_Bullet->mRotate = pTr->mRotate;
	pTr_Bullet->mRotationDir = { 0.0f, 0.0f, 1.0f };


	pBody_Bullet->Init();
	float angle = pTr->mRotate * PI / 180.0f;
	pBody_Bullet->mVel = { pEA->mShootSpeed * cosf(angle), pEA->mShootSpeed * sinf(angle) };
	pBody_Bullet->mPos = { pTr->mPos.x, pTr->mPos.y };

	pEA->mShootCurr = 0.0f;
}

void AIBehaviour::Update(float _frameTime)
{
	Component* c = gpComponentManager->mEnemyAI->GetHead();
	if (nullptr != c) {
		while (nullptr != c) {
			EnemyAI* pEA = static_cast<EnemyAI*>(c);
			Transform* pTr = pEA->mpOwner->GetComponent<Transform>();
			Body* pBody = pEA->mpOwner->GetComponent<Body>();

			if ("Drone" == pEA->mEnemyType) {

				if (pEA->mDroneActive) {
					pEA->mShootCurr += _frameTime;
					if (pEA->mShootCurr >= pEA->mShootGap) {
						Entity* pEntity = gpObjectFactory->CreateRuntimeObject("EnemyBullet.json");
						SetToEntity(pEntity, pTr, pEA);
						gpAudioManager->PlaySFX("EnemyBullet.mp3",0.4f,0.6f,-5.0f,5.0f);
						//pEntity->GetComponent<Collider>()->Init();
						for (auto c : pEntity->mComponents)
							c->Init();
					}
				}

				if (pEA->mDroneLOS) {
					pEA->mDroneLOS->GetComponent<Transform>()->mPos = pEA->mpOwner->GetComponent<Transform>()->mPos;
					pEA->mDroneActive = false;
				}
			}

			if ("Guard" == pEA->mEnemyType) {

				// GET SHAPE FOR CALCULATING OFFSET OF LOS
				Collider* pLOScollider = pEA->mLOS_Box->GetComponent<Collider>();
				Collider* pCollider = pEA->mpOwner->GetComponent<Collider>();
				ShapeAABB* pLOSaabb = static_cast<ShapeAABB*>(pLOScollider->mpShape);
				ShapeAABB* pAabb = static_cast<ShapeAABB*>(pCollider->mpShape);
				float offsetX = (pLOSaabb->mRight - pLOSaabb->mLeft) / 2.0f + (pAabb->mRight - pAabb->mLeft) / 2.0f;
				float offsetY = (pAabb->mTop - pAabb->mBottom) / 2.0f + 0.5f;

				Transform* pLOStransform = pEA->mLOS_Box->GetComponent<Transform>();
				if (pEA->mDir == EnemDir::ENEMR)
					pLOStransform->mPos = pTr->mPos + glm::vec3(offsetX, offsetY, 0.0f);
				else
					pLOStransform->mPos = pTr->mPos + glm::vec3(-offsetX, offsetY, 0.0f);


				// CHECK STATE
				if (pEA->GetPatrolingState()) {
      				// PATROL
					
					if (pEA->mCurrentWayPoint) {
						Transform* pWpTr = pEA->mCurrentWayPoint->GetComponent<Transform>();
						glm::vec2 dir = glm::vec2(pWpTr->mPos - pTr->mPos);
						float vel = (dir.x < 0.0f) ? -pEA->mGuardSpeed : pEA->mGuardSpeed;
						
						// CHANGE ROTATION
						if (vel < 0) pBody->pTr->mRotate = pEA->FlipEnemy(EnemDir::ENEML);
						else pBody->pTr->mRotate = pEA->FlipEnemy(EnemDir::ENEMR);
						
						pBody->mVel = { vel, 0 };
					}
				}

				if (pEA->GetChasingState()) {

					Transform* pWpTr = pEA->mCurrentWayPoint->GetComponent<Transform>();
					glm::vec2 dir = glm::vec2(pWpTr->mPos - pTr->mPos);

					if (dir.x < 0.0f) {
						pBody->pTr->mRotate = pEA->FlipEnemy(EnemDir::ENEML);
					}
					else {
						pBody->pTr->mRotate = pEA->FlipEnemy(EnemDir::ENEMR);
					}

					// SHOOT
					pEA->mShootCurr += _frameTime;
					if (pEA->mShootCurr >= pEA->mShootGap) {
						Entity* pEntity = gpObjectFactory->CreateRuntimeObject("EnemyBullet.json");
						gpAudioManager->PlaySFX("EnemyBullet.mp3", 0.4f, 0.6f, -5.0f, 5.0f);
						Body* pBody_Bullet = pEntity->GetComponent<Body>();
						Transform* pTr_Bullet = pEntity->GetComponent<Transform>();

						pTr_Bullet->mPos = { pTr->mPos.x , pTr->mPos.y, pTr->mPos.z };
						glm::vec2 dir = glm::vec2(gpSystemManager->mPlayerSystem->GetPlayerEntity()->GetComponent<Transform>()->mPos
										- pTr_Bullet->mPos);
						pTr_Bullet->mRotate = atan2f(dir.y, dir.x) * 180.0f / PI;
						pTr_Bullet->mRotationDir = { 0.0f, 0.0f, 1.0f };
						pTr->mDirtyFlag = true;

						pBody_Bullet->Init();

						float angle = pTr_Bullet->mRotate * PI / 180.0f;
						pBody_Bullet->mVel = { pEA->mShootSpeed * cosf(angle), pEA->mShootSpeed * sinf(angle) };
						pBody_Bullet->mPos = { pTr->mPos.x, pTr->mPos.y };



						pEA->mShootCurr = 0.0f;
					}
					pEA->SetChasing(false);
					pEA->SetPatroling(true);
				}
			}

			c = c->GetNext();
		}
	}
}