/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "Body.h"

#include "../InitData.h"
#include "Transform.h"
#include "Shape.h"
#include "Model.h"
#include "../Entity.h"
#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;
#include <glm/gtx/norm.hpp>
RTTR_REGISTRATION
{
	rttr::registration::class_<Body>("Body")
		 .property("Mass", &Body::mMass)
		 .property("useGravity", &Body::useGravity)
		 .property("constrainX", &Body::constrainX)
		 .property("constrainY", &Body::constrainY)
		 .property("Velocity", &Body::mVel);
}


Body::Body() : pTr(nullptr), pModel(nullptr), useGravity(true), constrainX(false), constrainY(false),
			mPos(0.0f), mPrevPos(0.0f), mVel(0.0f), mAcc(0.0f), mTotalForce(0.0f), constantVelocity(glm::vec2(0.0f)),
			isConstantVelocitySet(false), applyFriction(true)
{
	mComponentName = "Body";
}

Body::~Body()
{
}

void Body::Move(float rate)
{
	mPos += rate;
}

void Body::Init()
{
	if (mMass != 0.0f) {
		mInvMass = 1.0f / mMass;
	}
	else {
		mInvMass = 0.0f;
	}
	Initialize();
}

void Body::Initialize()
{
	for (auto x : mpOwner->mComponents) {
		if (x->mComponentName == "Transform") {
			pTr = static_cast<Transform*>(x);
			mPos.x = pTr->mPos.x;
			mPos.y = pTr->mPos.y;
			mPrevPos = mPos;

			/*ShapeAABB* rect = static_cast<ShapeAABB*>(mpShape);
			rect->mTop = mPos.y + pTr->mScale.y / 2.0f;
			rect->mBottom = mPos.y - pTr->mScale.y / 2.0f;
			rect->mLeft = mPos.x - pTr->mScale.x / 2.0f;
			rect->mRight = mPos.x + pTr->mScale.x / 2.0f;*/
		}
	}
	//*mDebugMatrix = glm::mat4(1.0f);
} 

void Body::SetConstantVelocity(glm::vec2 vel) {
	if (vel == glm::vec2(0.0f))
		return;

	// if constant velocity is not set and velocity set is different
	if (!isConstantVelocitySet || (vel.x + constantVelocity.x < 1.0f)) {
		isConstantVelocitySet = true;
		constantVelocity = vel;
		mVel = vel;
	}
}

void Body::SelfReset() {
	pTr = nullptr;
	pModel = nullptr;
	useGravity = true;
	//isTrigger = false;
	mPos = glm::vec2(0);
	mPrevPos = glm::vec2(0);
	mTotalForce = glm::vec2(0);
	mAcc = glm::vec2(0);
	mVel = glm::vec2(0);
}

void Body::SelfDelete() {
	SelfReset();
	gpComponentManager->mBody->Delete(this);
}

void Body::Clone(Component * rhs1)
{
	Body* rhs = static_cast<Body*>(rhs1);

	pTr = rhs->pTr;
	pModel = rhs->pModel;
	useGravity = rhs->useGravity;
	//isTrigger = false;
	mPos = rhs->mPos;
	mPrevPos = rhs->mPrevPos;
	mTotalForce = rhs->mTotalForce;
	mAcc = rhs->mAcc;
	mVel = rhs->mVel;
	mMass = rhs->mMass;
	mInvMass = rhs->mInvMass;
}