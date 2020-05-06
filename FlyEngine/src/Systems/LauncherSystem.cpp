/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "LauncherSystem.h"

#include "../Managers/ComponentManager.h"
#include "../Managers/SystemManager.h"
#include "../Systems/PlayerSystem.h"
#include "../Managers/ObjectFactory.h"
#include "../Managers/AudioManager.h"


#define PI 3.14159265358f

extern ComponentManager* gpComponentManager;
extern SystemManager * gpSystemManager;
extern ObjectFactory* gpObjectFactory;
extern AudioManager* gpAudioManager;

LauncherSystem::LauncherSystem()
{
}


LauncherSystem::~LauncherSystem()
{
}

void SetToEntity(Entity* _pEntity, Transform* _pTr, Launcher* _pLauncher);

void LauncherSystem::Update(float _frameTime) 
{
	Component* c = gpComponentManager->mLauncher->GetHead();
	Entity* playerEntity = gpSystemManager->mPlayerSystem->GetPlayerEntity();

	if (nullptr != playerEntity) {
		Transform* playerTransform = playerEntity->GetComponent<Transform>();
		while (nullptr != c) {
			Launcher* pLauncher = static_cast<Launcher*>(c);
			Transform* pTr = pLauncher->mpOwner->GetComponent<Transform>();
			Body* pBody_Bulletasdasaa;

			if (pLauncher->mMissileType == ShootBullet) {
				//Getting Rotation Angle
				float deltaX = playerTransform->mPos.x - pTr->mPos.x;
				float deltaY = playerTransform->mPos.y - pTr->mPos.y;

				pTr->mRotate = atan2f(deltaY, deltaX) * 180.0f / PI;
				pTr->mRotationDir = { 0.0f, 0.0f, 1.0f };
				pTr->mDirtyFlag = true;

				//Shooting Bullet in that Direction
				pLauncher->tempTime += _frameTime;
				if (pLauncher->tempTime >= pLauncher->mTimer) {
					Entity* mEntity = gpObjectFactory->CreateRuntimeObject("EnemyBullet.json");
					SetToEntity(mEntity, pTr, pLauncher);
					gpAudioManager->Play("LauncherLaunchingBullet.mp3");
					pBody_Bulletasdasaa = mEntity->GetComponent<Body>();

					for (auto c : mEntity->mComponents) {
						c->Init();
					}
				}
			}

			if (pLauncher->mMissileType == ShootHoming) {
				//Shooting HomingMissile in that Direction
				pLauncher->tempTime += _frameTime;
				if (pLauncher->tempTime >= pLauncher->mTimer) {
					Entity* mEntity = gpObjectFactory->CreateRuntimeObject("HomingMissile.json");
					SetToEntity(mEntity, pTr, pLauncher);
				}
				
			}

			if (pLauncher->mMissileType == Drone) {
				
				pLauncher->tempTime += _frameTime;
				if (pLauncher->tempTime >= pLauncher->mTimer && pLauncher->mDroneAlive.size() < 3 ) {
					Entity* mEntity = gpObjectFactory->CreateRuntimeObject("Drone.json"); // shoot drones
					EnemyAI* pEAI = mEntity->GetComponent<EnemyAI>();
					pEAI->mShootSpeed = 15.0f;
					pEAI->mShootGap = 2.0f;
					pLauncher->mDroneAlive.push_back(mEntity);
					mEntity->mOwnerEntity = pLauncher->mpOwner;
					SetToEntity(mEntity, pTr, pLauncher);

					Entity* mBoss = pLauncher->mpOwner->mOwnerEntity;
					if (nullptr != mBoss) {
						if (BOSS == mBoss->mType) {
							Boss* pBoss = mBoss->GetComponent<Boss>();

							//Adding Drones to Boss
							pBoss->mDrones.push_back(mEntity);
							pEAI->mBossOwner = mBoss;
						}
					}

					for (auto c : mEntity->mComponents) {
						c->Init();
					}
				}
			}

			c = c->GetNext();
		}
	}
}

void SetToEntity(Entity* _pEntity, Transform* _pTr, Launcher* _pLauncher)
{
	Body* pBody_Bullet = _pEntity->GetComponent<Body>();
	Transform* pTr_Bullet = _pEntity->GetComponent<Transform>();

	pTr_Bullet->mPos = { _pTr->mPos.x, _pTr->mPos.y, _pTr->mPos.z };
	pTr_Bullet->mRotate = _pTr->mRotate;
	pTr_Bullet->mRotationDir = { 0.0f, 0.0f, 1.0f };

	pBody_Bullet->Init();
	float angle = _pTr->mRotate * PI / 180.0f;
	pBody_Bullet->mVel = { _pLauncher->mMissileSpeed * cosf(angle), _pLauncher->mMissileSpeed * sinf(angle) };
	pBody_Bullet->mPos = { _pTr->mPos.x , _pTr->mPos.y };

	_pLauncher->tempTime = 0.0f;
}