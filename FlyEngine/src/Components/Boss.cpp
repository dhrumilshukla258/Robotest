/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Boss.h"
#include "../Component.h"
#include "Health.h"

#include "../Managers/ComponentManager.h"
#include "../Managers/ObjectFactory.h"

extern ComponentManager* gpComponentManager;
extern ObjectFactory* gpObjectFactory;

Boss::Boss()
{
	
}


Boss::~Boss()
{
}

void Boss::SelfDelete()
{
	gpObjectFactory->DeleteObject(mHealthBar->id);
	gpObjectFactory->DeleteObject(mExpression->id);
	//gpObjectFactory->DeleteObject(mDoor->id);
	mDoor = nullptr;
	mHealthBar = nullptr;
	mExpression = nullptr;
	for (Entity* e : mMinions) {
		e->mOwnerEntity = nullptr;
	}

	for (Entity* e : mDrones) {
		Entity* pFBPtcl = gpObjectFactory->CreateRuntimeObject("ParticleDronDie.json");//create enemy particles
		pFBPtcl->GetComponent<Transform>()->mPos = e->GetComponent<Transform>()->mPos; // particle in 

		Entity* pFBPtc2 = gpObjectFactory->CreateRuntimeObject("ParticleDronDie2.json");//create enemy particles
		pFBPtc2->GetComponent<Transform>()->mPos = e->GetComponent<Transform>()->mPos; // particle in 

		e->GetComponent<EnemyAI>()->mBossOwner = nullptr;
		gpObjectFactory->DeleteObject(e->id);
	}

	mDrones.clear();
	mMinions.clear();
	visitedOnce = false;
	killedAllLauncher = false;
	launcherDestroyed = 0;
	playBackwardAnimation = false;
	playForwardAnimation = false;
	
	gpComponentManager->mBoss->Delete(this);
}

void Boss::Clone(Component *)
{
}

void Boss::Init() 
{
	if (!visitedOnce) {
		mHealthBar = gpObjectFactory->CreateRuntimeObject("TextBox.json");
		Transform* pTr = mHealthBar->GetComponent<Transform>();
		pTr->mScale = { 0.47f, 1.0f, 1.0f };
		pTr->mPos = { 105.56, 8.27, 4.0 };
		Text* pText = mHealthBar->GetComponent<Text>();
		pText->mTextColor = { 0.0f, 1.0f, 1.0f };
		pText->mTextAppear = true;
		pText->mStr = "----------";
		visitedOnce = true;

		mExpression = gpObjectFactory->CreateRuntimeObject("BossFace.json");
		Transform * mExp_pTr = mExpression->GetComponent<Transform>();

		//Boss Position when small
		mExp_pTr->mPos = { 113.0f, 3.95f, -6.0f };

		//Offset
		mExp_pTr->mPos.y -= 2.00f;
		mExp_pTr->mPos.z += 5.0f;

		mDoor = gpObjectFactory->CreateRuntimeObject("Door.json");
		Model * mDoor_model = mDoor->GetComponent <Model>();
		mDoor_model->mModelFile = "White_Light.obj";
		mDoor_model->mCol = { 1.0f, 1.0f, 1.0f };

		Transform * mDoor_ptr = mDoor->GetComponent<Transform>();
		mDoor_ptr->mPos = { 151.8f, -16.5f, -1.2f };
		mDoor_ptr->mScale = { 0.5f, 4.3f, 3.0f };

		Body * mDoor_pbody = mDoor->GetComponent<Body>();
		mDoor_pbody->mPos = { 151.8f, -16.5f };

		Collider * mDoor_pColl = mDoor->GetComponent<Collider>();
		Door * mDoor_pDoor = mDoor->GetComponent<Door>();
		mDoor_pDoor->mDoorType = PressOnce;

		mDoor_pbody->Init();
		mDoor_model->Init();
		mDoor_pColl->Init();
		mDoor_pDoor->Init();

		/*FrameHealtBar = gpObjectFactory->CreateRuntimeObject("ForeGround.json");//Attempt to stablish the background for the healtbar so its possible to erase it after
		Transform* pTr2 = FrameHealtBar->GetComponent<Transform>();
		pTr2->mPos = { 113.0, 10.0, 0.0 };
		pTr2->mScale = { 3.0, 0.1, 0.3 };
	
		Model* pTr3 = FrameHealtBar->GetComponent<Model>();
		pTr3->mCol = { 0.0 , 0.0 , 0.0 };*/
	}
}



void Boss::DeleteChildReference(unsigned int id)
{
	Entity * e = nullptr;
	for (auto d : mMinions) {
		if (d->id == id)
			e = d;
	}

	std::vector<Entity *>::iterator itr = std::find(mMinions.begin(), mMinions.end(), e);

	if (itr != mMinions.end()) {
		++launcherDestroyed;
		mMinions.erase(itr);
	}

	if (launcherDestroyed == 4) {
		killedAllLauncher = true;
		playForwardAnimation = true;
	}

	if ( launcherDestroyed == 8) {
		killedAllLauncher = true;
		playForwardAnimation = true;		
		Health* pH = mpOwner->GetComponent<Health>();
		pH->mHealthLeft = 49.0f;
	}
}