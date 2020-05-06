/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "BossSystem.h"
#include "../Entity.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/ObjectFactory.h"

extern ComponentManager* gpComponentManager;
extern ObjectFactory* gpObjectFactory;

BossSystem::BossSystem()
{

}


BossSystem::~BossSystem()
{
}

void BossSystem::Update(float _frameTime)
{
	Component* c = gpComponentManager->mBoss->GetHead();

	while (nullptr != c) {
		Boss* pBoss = static_cast<Boss*>(c);
		Transform* pTr = pBoss->mpOwner->GetComponent<Transform>();
		Body* pBody = pBoss->mpOwner->GetComponent<Body>();
		Health* pHealth = pBoss->mpOwner->GetComponent<Health>();

		bool drone_1 = false;
		bool drone_2 = false;
		bool bullet_1 = false;
		bool bullet_2 = false;

		glm::vec3 pos_1_drone = { 101.0f, 29.0f, 0.0f };
		glm::vec3 pos_2_drone = { 124.0f, 29.0f, 0.0f };
		glm::vec3 pos_1_bullet = { 124.0f, -7.75f, 0.0f };
		glm::vec3 pos_2_bullet = { 101.0f, -7.75f, 0.0f };
		
		if ( ( pBoss->launcherDestroyed == 0 && pHealth->mHealthLeft == 100.0f ) ||
			 ( pBoss->launcherDestroyed == 4 && fabs(pHealth->mHealthLeft - 50.0f) < 1.0f) )
		{
			if (pBoss->mMinions.size() == 0) {

				for (auto d : pBoss->mMinions) {
					Launcher * pLauncher = d->GetComponent<Launcher>();

					if (pLauncher->bossSystemNeedsThis == 1)
						drone_1 = true;
					if (pLauncher->bossSystemNeedsThis == 2)
						drone_2 = true;

					if (pLauncher->bossSystemNeedsThis == 3)
						bullet_1 = true;
					if (pLauncher->bossSystemNeedsThis == 4)
						bullet_2 = true;
				}

				if (!drone_1) {
					Entity* mEntity = gpObjectFactory->CreateRuntimeObject("Launcher.json");
					mEntity->mOwnerEntity = pBoss->mpOwner;
					pBoss->mMinions.push_back(mEntity);

					Body* pBody = mEntity->GetComponent<Body>();
					pBody->mPos = { pos_1_drone.x, pos_1_drone.y };

					Transform* pTr = mEntity->GetComponent<Transform>();
					pTr->mScale = { 1.3f, 1.0f, 0.8f };
					pTr->mPos = pos_1_drone;

					Launcher* pLaunch = mEntity->GetComponent<Launcher>();
					pLaunch->mMissileType = Drone;
					pLaunch->mMissileSpeed = 1.0f;
					pLaunch->bossSystemNeedsThis = 1;

					for (auto c : mEntity->mComponents)
						c->Init();
				}
				if (!drone_2) {
					Entity* mEntity = gpObjectFactory->CreateRuntimeObject("Launcher.json");
					mEntity->mOwnerEntity = pBoss->mpOwner;
					pBoss->mMinions.push_back(mEntity);

					Body* pBody = mEntity->GetComponent<Body>();
					pBody->mPos = { pos_2_drone.x, pos_2_drone.y };

					Transform* pTr = mEntity->GetComponent<Transform>();
					pTr->mScale = { 1.3f, 1.0f, 0.8f };
					pTr->mPos = pos_2_drone;

					Launcher* pLaunch = mEntity->GetComponent<Launcher>();
					pLaunch->mMissileType = Drone;
					pLaunch->mMissileSpeed = 1.0f;
					pLaunch->bossSystemNeedsThis = 2;
					for (auto c : mEntity->mComponents)
						c->Init();
				}
				if (!bullet_1) {
					Entity* mEntity = gpObjectFactory->CreateRuntimeObject("Launcher.json");
					mEntity->mOwnerEntity = pBoss->mpOwner;
					pBoss->mMinions.push_back(mEntity);

					Body* pBody = mEntity->GetComponent<Body>();
					pBody->mPos = { pos_1_bullet.x, pos_1_bullet.y };

					Transform* pTr = mEntity->GetComponent<Transform>();
					pTr->mScale = { 0.4f, 0.5f, 0.4f };
					pTr->mPos = pos_1_bullet;

					Launcher* pLaunch = mEntity->GetComponent<Launcher>();
					pLaunch->mMissileType = ShootBullet;
					pLaunch->mMissileSpeed = 15.0f;
					pLaunch->bossSystemNeedsThis = 3;
					for (auto c : mEntity->mComponents)
						c->Init();
				}
				if (!bullet_2) {
					Entity* mEntity = gpObjectFactory->CreateRuntimeObject("Launcher.json");
					mEntity->mOwnerEntity = pBoss->mpOwner;
					pBoss->mMinions.push_back(mEntity);

					Body* pBody = mEntity->GetComponent<Body>();
					pBody->mPos = { pos_2_bullet.x, pos_2_bullet.y };

					Transform* pTr = mEntity->GetComponent<Transform>();
					pTr->mScale = { 0.4f, 0.5f, 0.4f };
					pTr->mPos = pos_2_bullet;

					Launcher* pLaunch = mEntity->GetComponent<Launcher>();
					pLaunch->mMissileType = ShootBullet;
					pLaunch->mMissileSpeed = 30.0f;
					pLaunch->bossSystemNeedsThis = 4;
					for (auto c : mEntity->mComponents)
						c->Init();
				}
			}
		}
		

		//Here add the health bar system
		Text* pText = pBoss->mHealthBar->GetComponent<Text>();
		int x = (int)pHealth->mHealthLeft;
		int livesBoss = x / 10;
		string porcentage = "";
		for (int i = 0 ; i < livesBoss ; ++i){ 
			porcentage +="-";
		}
		pText->mStr = porcentage;// "Boss Health : " + std::to_string(x);
		pText->mTextColor =
			livesBoss > 9 ? glm::vec3(0, 1, 1) : //green electro
			livesBoss > 7 ? glm::vec3(0, 1, 0) ://green
			livesBoss > 5 ? glm::vec3(1, 1, 0) ://yellow
			livesBoss > 3 ? glm::vec3(1, 0.5, 0) ://orange
			glm::vec3(1, 0, 0);//red


		//Forward backward Animation
		if (pBoss->playForwardAnimation) {
			if (pTr->mPos.z <= 3.1f) {
				pTr->mPos.z += 0.091f;
				
				pTr->mPos.y -= 0.03465f;
				pBody->mPos.y -= 0.03465f;

				pTr->mScale += 0.014;
				

				Transform * exp_pTr = pBoss->mExpression->GetComponent<Transform>();
				exp_pTr->mScale.x += 0.01f;
				exp_pTr->mScale.z -= 0.013f;

				exp_pTr->mPos.y -= 0.04765f;
				exp_pTr->mPos.z += 0.12f;
			}
			else
				pBoss->playForwardAnimation = false;
		}

		if (pBoss->playBackwardAnimation) {
			if (pTr->mPos.z >= -6.0f) {
				pTr->mPos.z -= 0.091f;
				
				pTr->mPos.y += 0.03465f;
				pBody->mPos.y += 0.03465f;

				pTr->mScale -= 0.014;

				Transform * exp_pTr = pBoss->mExpression->GetComponent<Transform>();
				exp_pTr->mScale.x -= 0.01f;
				exp_pTr->mScale.z += 0.013f;

				exp_pTr->mPos.y += 0.04765f;
				exp_pTr->mPos.z -= 0.12f;
			}
			else
				pBoss->playBackwardAnimation = false;
		}

		c = c->GetNext();
	}
}