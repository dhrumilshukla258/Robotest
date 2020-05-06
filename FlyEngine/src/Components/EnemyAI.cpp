/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "EnemyAI.h"

#include "../Entity.h"
#include "Body.h"
#include "Transform.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/ObjectFactory.h"

extern ComponentManager* gpComponentManager;
extern ObjectFactory* gpObjectFactory;
extern bool gEditMode;

RTTR_REGISTRATION
{
	rttr::registration::class_<EnemyAI>("EnemyAI")
		 .property("EnemyType", &EnemyAI::mEnemyType)
		 .property("ShootSpeed", &EnemyAI::mShootSpeed)
		 
		 .property("LOS_Box", &EnemyAI::mLOS_Box)
		 .property("WayPoints", &EnemyAI::wpVec)
		 .property("GuardSpeed", &EnemyAI::mGuardSpeed)

		 .property("ShootGap", &EnemyAI::mShootGap)
		 .property("DroneActive", &EnemyAI::mDroneActive)
		 .property("DroneLOS", &EnemyAI::mDroneLOS);
}

EnemyAI::EnemyAI() : mDroneLOS(nullptr), mLOS_Box(nullptr), wpVec(), mShootCurr(0.0f), mShootGap(0.0f), mDroneActive(true), mState(nullptr),
					mCurrentWayPoint(nullptr), mCurrentWPIndex(0), mShootSpeed(0.0f), mGuardSpeed(0.0f)
{
	mComponentName = "EnemyAI";
}

EnemyAI::~EnemyAI()
{
	if (mState) delete mState;
}

void EnemyAI::Init()
{
	if ("Drone" == mEnemyType) {
		if (mDroneLOS) {
			mDroneLOS->mOwnerEntity = mpOwner;
			mpOwner->mChildEntities.push_back(mDroneLOS);
		}
	}

	if ("Guard" == mEnemyType) {
		mLOS_Box->mOwnerEntity = mpOwner;
		mpOwner->mChildEntities.push_back(mLOS_Box);

		Transform* pTr = mpOwner->GetComponent<Transform>();
		if (gEditMode) {
			mLOS_Box->GetComponent<Transform>()->mPos = pTr->mPos + pTr->mScale / 2.0f;
		}

		unsigned int i = 1;
		for (auto wp : wpVec) {
			wp->mOwnerEntity = mpOwner;
			mpOwner->mChildEntities.push_back(wp);
			
			if (gEditMode) {
				wp->GetComponent<Transform>()->mPos = pTr->mPos + pTr->mScale / 2.0f + glm::vec3((float)(5 * i), 0.0f, 0.0f);
				++i;
			}
		}
		

		mState = new EnemyState();
		mState->PatrolingState = true;
		if(0 != wpVec.size())
			mCurrentWayPoint = wpVec.at(0);
		//mLOS_Box->mParent = this->mpOwner;
		/*for (auto wp : wpVec) {
			wp->mParent = this->mpOwner;
		}*/

		mDir = EnemDir::ENEMR;
		tweenL = tweeny::from(0.0f).to(-180.0f).during(100.0f);
		tweenR = tweeny::from(-180.0f).to(0.0f).during(100.0f);
		tweenR.seek(1); // assuming starting direction to be east
	}
}

void EnemyAI::DeleteChildReference(unsigned int id)
{
	if (mDroneLOS) {
		if (mDroneLOS->id == id) {
			mDroneLOS = nullptr;
		}
	}
	else {
		if (mLOS_Box) {
			if (mLOS_Box->id == id) {
				mLOS_Box = nullptr;
			}
		}
		else {
			for (auto wp : wpVec) {
				if (wp->id == id) {
					wp = nullptr;
				}
			}
		}
	}
}

void EnemyAI::SelfDelete()
{
	if (nullptr != mBossOwner) {
		Boss * pBoss = mBossOwner->GetComponent<Boss>();
		
		std::vector<Entity*>::iterator it = std::find(pBoss->mDrones.begin(), pBoss->mDrones.end(), mpOwner);

		if (pBoss->mDrones.end() != it) {
			pBoss->mDrones.erase(it);
		}
	}

	mBossOwner = nullptr;
	mShootCurr = 0.0f;
	mShootGap = 0.0f;
	mShootSpeed = 0.0f;
	mGuardSpeed = 0.0f;
	mEnemyType = "";
	mCurrentWPIndex = 0;
	mDroneActive = true;
	mState = nullptr;

	/*if (mLOS_Box)
		gpObjectFactory->DeleteObject(mLOS_Box->id);
	mLOS_Box = nullptr;*/

	/*for (auto wp : wpVec) {
		gpObjectFactory->DeleteObject(wp->id);
	}*/
	wpVec.clear();
	gpComponentManager->mEnemyAI->Delete(this);
}

float EnemyAI::FlipEnemy(EnemDir _dir) {
	if (_dir == mDir) {
		if (_dir == EnemDir::ENEML) {
			if (tweenL.progress() < 1) {
				mpOwner->GetComponent<Body>()->pTr->mDirtyFlag = true;
				return tweenL.step(0.05f);
			}
			else
				return -180.0f;
		}

		else {
			if (tweenR.progress() < 1) {
				mpOwner->GetComponent<Body>()->pTr->mDirtyFlag = true;
				return tweenR.step(0.05f);
			}
			else
				return 0.0f;
		}
	}
	else {
		if (_dir == EnemDir::ENEMR) {
			mDir = EnemDir::ENEMR;
			//LOG_INFO("ConsoleLogger", "Right");
			tweenR.seek(0);
			return -180.0f;
		}
		else {
			mDir = EnemDir::ENEML;
			//LOG_INFO("ConsoleLogger", "Left");
			tweenL.seek(0);
			return 0.0f;
		}
	}
}

void EnemyAI::ResolveWayPoint(Entity* _wayPoint)
{
	if (mCurrentWayPoint) {
		if (!_wayPoint) return;
		if (_wayPoint == mCurrentWayPoint) {
			
			for (unsigned int i = 0; i < wpVec.size(); ++i) {
				if (mCurrentWPIndex == i) {
					if (wpVec.size() - 1 == i) {
						mCurrentWayPoint = wpVec[0];
						mCurrentWPIndex = 0;
						//LOG_INFO("ConsoleLogger", "{}", mCurrentWPIndex);
						return;
					}
					else {
						mCurrentWayPoint = wpVec[++mCurrentWPIndex];
						//LOG_INFO("ConsoleLogger", "{}", mCurrentWPIndex);
						return;
					}
				}
			}

		}
	}
	else {
		if (!wpVec.empty()) {
			mCurrentWPIndex = 0;
			mCurrentWayPoint = wpVec[0];
		}
	}
}