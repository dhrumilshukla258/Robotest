/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "PhysicsSystem.h"

#include "../Components/Shape.h"
#include "../Managers/CollisionManager.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/FrameRateController.h"
#include "../Managers/GameplayManager.h"

extern CollisionManager* gpCollisionManager;
extern FrameRateController* gpFrameRateController;
extern ComponentManager* gpComponentManager;
extern GameplayManager* gpManager;

PhysicsSystem::PhysicsSystem()
{
	gravity = glm::vec2(0.0f, -100.0f);
}

void PhysicsSystem::ResetForces() {
	Component* c = gpComponentManager->mBody->GetHead();
	while (nullptr != c) {
		Body *pBody = static_cast<Body*>(c);
		if (pBody) {
			pBody->mTotalForce = glm::vec2(0);
		}
		c = c->GetNext();
	}
}

int impulseIterations = 3;

void PhysicsSystem::Update(float _deltaTime) {

	fixedDeltaTime = _deltaTime;
	//gravity = glm::vec2(0.0f, gpScriptManager->UpdateGravity());
	
	//==== check for collisions
	gpCollisionManager->Reset();

	Component* c1 = gpComponentManager->mCollider->GetHead();
	Component* c2 = nullptr;
	Collider* pCol1 = nullptr;
	Collider* pCol2 = nullptr;
	glm::vec2 pos1, pos2;

	while (nullptr != c1) {
		c2 = c1->GetNext();
		pCol1 = static_cast<Collider*>(c1);

		while (nullptr != c2) {
			pCol2 = static_cast<Collider*>(c2);
			//LOG_INFO("ConsoleLogger", "pBody2 -- {}",c2->mpOwner->id);
			int imSum = 2;
			//Update Here coz we have continue condition
			c2 = c2->GetNext();

			if (pCol1->mpOwner->mType == FIRE_BULLET && ( pCol2->mpOwner->mType == FIRE_BULLET ||
													   pCol2->mpOwner->mType == ICE_BULLET  ||
													   pCol2->mpOwner->mType == ENEMY_BULLET ||
													   pCol2->mpOwner->mType == DOOR_CLOSE_TRIGGER ||
													   pCol2->mpOwner->mType == ENEMY_TRIGGER ||
													   pCol2->mpOwner->mType == ENEMY_WAY_POINT ||
													   pCol2->mpOwner->mType == FIRE_GUN ||
													   pCol2->mpOwner->mType == TEXT_TRIGGER  ||
													   pCol2->mpOwner->mType == CAMERA_TRIGGER ||
													   pCol2->mpOwner->mType == TRIGGER_BOX ||
													   pCol2->mpOwner->mType == DRONE_TRIGEGR) ||
				
				pCol2->mpOwner->mType == FIRE_BULLET && ( pCol1->mpOwner->mType == ENEMY_TRIGGER || 
														  pCol1->mpOwner->mType == ENEMY_WAY_POINT ||
														  pCol1->mpOwner->mType == DOOR_CLOSE_TRIGGER ||
										     			  pCol1->mpOwner->mType == FIRE_GUN ||
														  pCol1->mpOwner->mType == TEXT_TRIGGER || 
														  pCol1->mpOwner->mType == CAMERA_TRIGGER ||
														  pCol2->mpOwner->mType == TRIGGER_BOX ||
														  pCol1->mpOwner->mType == DRONE_TRIGEGR) ||

				pCol1->mpOwner->mType == ICE_BULLET  && ( pCol2->mpOwner->mType == FIRE_BULLET ||
													   pCol2->mpOwner->mType == ICE_BULLET  ||
													   pCol2->mpOwner->mType == ENEMY_BULLET) ||

				pCol1->mpOwner->mType == ENEMY_BULLET && (pCol2->mpOwner->mType == FIRE_BULLET ||
													   pCol2->mpOwner->mType == ICE_BULLET  ||
													   pCol2->mpOwner->mType == ENEMY_BULLET) ||

				pCol1->mpOwner->mType == ENEMY_BULLET && ( pCol2->mpOwner->mType == DRONE ||
														   pCol2->mpOwner->mType == LAUNCHER ||
														   pCol2->mpOwner->mType == ENEMY ||
														   pCol2->mpOwner->mType == DOOR_CLOSE_TRIGGER ||
														   pCol2->mpOwner->mType == ENEMY_WAY_POINT ||
														   pCol2->mpOwner->mType == TEXT_TRIGGER ||
														   pCol2->mpOwner->mType == CAMERA_TRIGGER ||
														   pCol2->mpOwner->mType == ENEMY_TRIGGER ||
														   pCol2->mpOwner->mType == DRONE_TRIGEGR) ||

				pCol2->mpOwner->mType == ENEMY_BULLET && (pCol1->mpOwner->mType == DRONE ||
														  pCol1->mpOwner->mType == LAUNCHER ||
													      pCol1->mpOwner->mType == ENEMY ||
														  pCol1->mpOwner->mType == DOOR_CLOSE_TRIGGER ||
														  pCol1->mpOwner->mType == TEXT_TRIGGER ||
														  pCol1->mpOwner->mType == CAMERA_TRIGGER ||
														  pCol1->mpOwner->mType == ENEMY_WAY_POINT ||
														  pCol1->mpOwner->mType == ENEMY_TRIGGER ||
														  pCol1->mpOwner->mType == DRONE_TRIGEGR) ||

				pCol1->mpOwner->mType == DRONE && ( pCol2->mpOwner->mType == ENEMY_BULLET ||
													pCol2->mpOwner->mType == LAUNCHER ) ||

				pCol2->mpOwner->mType == DRONE && ( pCol1->mpOwner->mType == ENEMY_BULLET ||
													pCol1->mpOwner->mType == LAUNCHER) ||

				pCol1->mpOwner->mType == PLAYER && ( pCol2->mpOwner->mType== FIRE_BULLET ||
													 pCol2->mpOwner->mType == ICE_BULLET ) ||

				pCol2->mpOwner->mType == PLAYER && ( pCol1->mpOwner->mType == FIRE_BULLET ||
													 pCol1->mpOwner->mType == ICE_BULLET)
				/*
				pCol1->mpOwner == pCol2->mpOwner->mParent ||	
				pCol2->mpOwner == pCol1->mpOwner->mParent*/
				)
			{
				continue;
			}

			if (pCol1->mpOwner->mType == BOSS) {
				Boss* pBoss = pCol1->mpOwner->GetComponent<Boss>();
				if (!pBoss->killedAllLauncher)
					continue;
			}
			if (pCol2->mpOwner->mType == BOSS) {
				Boss* pBoss = pCol2->mpOwner->GetComponent<Boss>();
				if (!pBoss->killedAllLauncher)
					continue;
			}

			if (pCol1->isTrigger) {
				pos1.x = pCol1->mpTr->mPos.x;
				pos1.y = pCol1->mpTr->mPos.y;
			}
			else {
				pos1 = pCol1->mpBody->mPos;
				if(pCol1->mpBody->mInvMass == 0)
					--imSum;
			}
			if (pCol2->isTrigger) {
				pos2.x = pCol2->mpTr->mPos.x;
				pos2.y = pCol2->mpTr->mPos.y;
			}
			else {
				pos2 = pCol2->mpBody->mPos;
				if (pCol2->mpBody->mInvMass == 0)
					--imSum;
			}
			// if sum of inverse masses = 0, then don't check for collision
			if (imSum == 0)
			{
				continue;
			}
			gpCollisionManager->CheckCollisionAndGenerateContact(
				pCol1->mpShape,
				pos1,
				pCol2->mpShape,
				pos2
			);
		}

		//Update Here coz we have continue condition
		c1 = c1->GetNext();
	}

	//==== integrate forces
	Component* c = gpComponentManager->mBody->GetHead();
	Body* pBody = nullptr;
	while (nullptr != c) {
		pBody = static_cast<Body*>(c);
		c = c->GetNext();
		if (pBody->mInvMass == 0.0f)
			continue;
		
		
		glm::vec2 grav = gravity;
		if (!pBody->useGravity) {
			grav.x = grav.y = 0.0f;
		}
		// compute acceleration
		pBody->mAcc = pBody->mTotalForce * pBody->mInvMass + grav;

		// integrate the velocity
		pBody->mVel += pBody->mAcc * _deltaTime;

		// super jugaad
		pBody->mTotalForce.y = pBody->mTotalForce.y > 25000.0f ? 0.0f : pBody->mTotalForce.y;

	}

	//==== apply impulses
	for(int i = 0; i < impulseIterations; ++i)
	for (auto c : gpCollisionManager->mContacts) {
		if (c->mpCollider[0]->isTrigger ||
			c->mpCollider[1]->isTrigger) {
			continue;
		}
		c->ApplyImpulse();

		// apply constraints to velocity
		c->A->mVel.x = c->A->constrainX ? 0.0f : c->A->mVel.x;
		c->A->mVel.y = c->A->constrainY ? 0.0f : c->A->mVel.y;
		c->B->mVel.x = c->B->constrainX ? 0.0f : c->B->mVel.x;
		c->B->mVel.y = c->B->constrainY ? 0.0f : c->B->mVel.y;
	}

	//==== integrate velocity
	c = gpComponentManager->mBody->GetHead();
	pBody = nullptr;
	while (nullptr != c) {
		pBody = static_cast<Body*>(c);
		//Update Here coz we have continue condition
		c = c->GetNext();
		// save current position
		pBody->mPrevPos = pBody->mPos;

		if (pBody->mInvMass == 0.0f)
			continue;
		
		// integrate the position
		pBody->mPos += pBody->mVel * _deltaTime;

		
	}

	//==== position correction
	for (auto c : gpCollisionManager->mContacts) {
		if (c->mpCollider[0]->isTrigger ||
			c->mpCollider[1]->isTrigger)
		{
			continue;
		}
		c->CorrectPenetration();
	}

	//==== Will use this for game behaviour now on
	gpManager->CollisionUpdate(gpCollisionManager->mContacts);
}

void PhysicsSystem::InterpolateState(float blendingFactor)
{
	// setting  transform now
	Component* c = gpComponentManager->mBody->GetHead();
	Body *pBody;
	while (nullptr != c) {
		pBody = static_cast<Body*>(c);
		//Update Here coz we have continue condition
		// this leads to choppiness
		/*pTr->mPos.x = pBody->mPos.x;
		pTr->mPos.y = pBody->mPos.y;*/
		
		// TODO should also work for mRotate
		if (pBody->mPos != pBody->mPrevPos)
			pBody->pTr->mDirtyFlag = true;

		pBody->pTr->mPos.x = pBody->mPos.x * blendingFactor + pBody->mPrevPos.x *(1 - blendingFactor);
		pBody->pTr->mPos.y = pBody->mPos.y * blendingFactor + pBody->mPrevPos.y *(1 - blendingFactor);
		c = c->GetNext();
	}
	
}

PhysicsSystem::~PhysicsSystem()
{
}

/* ----------------------------------- DON'T EVER LOOK BELOW THIS LINE OF CODE ----------------------------------------

	auto vecs = gpComponentManager->mBody->mActiveVec.begin();
	auto vece = gpComponentManager->mBody->mActiveVec.end();
	for (auto &vitr = vecs; vitr != vece; vitr++){
		auto vstart = vitr;
			vstart++;
		for (auto vitr2 = vstart; vitr2 != vece; vitr2++) {

		}
	}
	return;

	unsigned int size = gpComponentManager->mBody->mActiveVec.size();
	auto list = gpComponentManager->mBody->mActiveVec;

	for (unsigned int i = 0; i < size; ++i) {
		//auto& x = gpComponentManager->mBody->mActiveVec[i];

		for (unsigned int j = i + 1; j < size; ++j) {
			list[j];
			continue;
			if (gpComponentManager->mBody->mActiveVec[i]->mpOwner->mType == gpComponentManager->mBody->mActiveVec[j]->mpOwner->mType) continue;

			continue;
			Body* pBody1 = static_cast<Body*>(gpComponentManager->mBody->mActiveVec[i]);
			Body* pBody2 = static_cast<Body*>(gpComponentManager->mBody->mActiveVec[j]);

			gpCollisionManager->CheckCollisionAndGenerateContact(
				static_cast<Body*>(pBody1)->mpShape,
				static_cast<Body*>(pBody1)->mPos,
				static_cast<Body*>(pBody2)->mpShape,
				static_cast<Body*>(pBody2)->mPos
			);

		}
	}




	// currently checking for all bodies in component manager active
	// (this assumes that at least a pair of bodies are present at all times)
	// N^2 system for collision testing
	// (Better methods exist if needed)



	for (auto itr1 = start; itr1 != end; ++itr1) {
		continue;
		auto start2 = itr1; // this might not work as expected
		++start2;
		for (auto itr2 = start2; itr2 != end; ++itr2) {
			if (itr1->second->mpOwner->mType == itr2->second->mpOwner->mType)
				continue;

			gpCollisionManager->CheckCollisionAndGenerateContact(
				static_cast<Body*>(itr1->second)->mpShape,
				static_cast<Body*>(itr1->second)->mPos,
				static_cast<Body*>(itr2->second)->mpShape,
				static_cast<Body*>(itr2->second)->mPos
			);
		}
	}*/
