/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Door.h"
#include "Body.h"
#include "Shape.h"
#include "../Entity.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/ObjectFactory.h"
extern ComponentManager* gpComponentManager;
extern ObjectFactory* gpObjectFactory;

RTTR_REGISTRATION
{
	rttr::registration::enumeration<DoorType>("DoorType")
	(
		rttr::value("PressOnce", DoorType::PressOnce),
		rttr::value("PressConstantly", DoorType::PressConstantly),
		rttr::value("PressMultipleTimes", DoorType::PressMultipleTimes)
	);

	rttr::registration::class_<Door>("Door")
		 .constructor()
		 .property("DoorType", &Door::mDoorType)
		 .property("wantToClose", &Door::wantToClose);
}

Door::Door()
{
	mComponentName = "Door";
}

Door::~Door()
{
}

void Door::Init() {
	Collider* pCol = mpOwner->GetComponent<Collider>();
	ShapeAABB* rect = static_cast<ShapeAABB*>(pCol->mpShape);
	doorLength = 2.0f * rect->mTop;

	if (wantToClose) {
		isClose = true;
		triggerToClose = gpObjectFactory->CreateRuntimeObject("TriggerBox.json");
		triggerToClose->mParent = mpOwner;

		triggerToClose->mType = DOOR_CLOSE_TRIGGER;
		
		Transform * doorTrigger_pTr = triggerToClose->GetComponent<Transform>();
		doorTrigger_pTr->mPos = {81.6f,-5.7f, -0.75f};
		doorTrigger_pTr->mScale = { 18.25f, 1.0f, 0.25f };
		
		Collider * doorTrigger_pCollider = triggerToClose->GetComponent<Collider>();
		doorTrigger_pCollider->Init();

		wantToClose = false;
	}
}

void Door::SelfDelete() {
	mKeyHit = false;
	mKeyForMultipleHit = false;
	
	isOpen = false;
	isClose = true;

	doorLength = 0.0f;
	tempDoorLength = 0.0f;

	closeDoor = false;
	wantToClose = false;
	if (triggerToClose != nullptr) {
		gpObjectFactory->DeleteObject(triggerToClose->id);
		triggerToClose = nullptr;
	}
	gpComponentManager->mDoor->Delete(this);
}

void Door::Clone(Component * rhs1) {
	Door* rhs = static_cast<Door*>(rhs1);

	 mKeyHit = rhs->mKeyHit;
	 mKeyForMultipleHit = rhs->mKeyForMultipleHit;
	 mDoorType = rhs->mDoorType;
	 isOpen = rhs->isOpen;
	 isClose = rhs->isClose;
	 doorLength = rhs ->doorLength;
	 tempDoorLength = rhs->tempDoorLength;

}