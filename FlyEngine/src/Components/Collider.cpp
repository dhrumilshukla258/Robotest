/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Collider.h"
#include "Model.h"
#include "Stencil_Toon_Model.h"
#include "Transform.h"
#include "Body.h"
#include "Shape.h"
#include "../Entity.h"
#include "../Managers/ComponentManager.h"
#include "../Utilities/ShapeStructure.h"
extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<Collider>("Collider")
		 .constructor()
		 .property("Shape", &Collider::mpShape)
		 .property("e", &Collider::coeffRestitution)
		 .property("fs", &Collider::coeffStaticFriction)
		 .property("fd", &Collider::coeffDynamicFriction)
		 .property("isTrigger", &Collider::isTrigger);
}

Collider::Collider() : mpShape(nullptr), isTrigger(false)
{
	coeffRestitution = 0.2f; // bounce
	coeffStaticFriction = 0.2f; // to start sliding
	coeffDynamicFriction = 0.12f; // while sliding

	//*mDebugMatrix = glm::mat4(1.0f);
	mDebugMatrix = reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16));

	mComponentName = "Collider";

}

void Collider::Init() {

	mpTr = mpOwner->GetComponent<Transform>();
	Model* pModel = mpOwner->GetComponent<Model>();
	Stencil_Toon_Model* pSTMModel = mpOwner->GetComponent<Stencil_Toon_Model>();
	//this is called on clonning, but we havent referenced mpshape.
	if (!mpShape) {
		//this case is after clonning when hasn't reference to  mpShape
	}// this 
	if (pModel && !isTrigger) {
		// TODO make generic for Spheres and AABBs
		ShapeAABB* rect = static_cast<ShapeAABB*>(mpShape);

		rect->mTop = pModel->y_max *mpTr->mScale.z;
		rect->mBottom = pModel->y_min *mpTr->mScale.z;
		rect->mLeft = pModel->x_min *mpTr->mScale.x;
		rect->mRight = pModel->x_max *mpTr->mScale.x;
	}
	else if (pSTMModel || !isTrigger) {
		// TODO make generic for Spheres and AABBs
		ShapeAABB* rect = static_cast<ShapeAABB*>(mpShape);

		rect->mTop = pSTMModel->y_max *mpTr->mScale.z;
		rect->mBottom = pSTMModel->y_min *mpTr->mScale.z;
		rect->mLeft = pSTMModel->x_min *mpTr->mScale.x;
		rect->mRight = pSTMModel->x_max *mpTr->mScale.x;
	}
	else if (pModel || pSTMModel && isTrigger) {
		// do nothing in this case because the trigger bounds
		// are serialized already
	}
	else {
		ShapeAABB* rect = static_cast<ShapeAABB*>(mpShape);
		rect->mTop = mpTr->mPos.y + mpTr->mScale.y / 2.0f;
		rect->mBottom = mpTr->mPos.y - mpTr->mScale.y / 2.0f;
		rect->mLeft = mpTr->mPos.x - mpTr->mScale.x / 2.0f;
		rect->mRight = mpTr->mPos.x + mpTr->mScale.x / 2.0f;
	}


	mpBody = mpOwner->GetComponent<Body>();
	if (mpBody == nullptr) {
		isTrigger = true;
	}
}

Collider::~Collider()
{
	_aligned_free(mDebugMatrix);
}

void Collider::SelfDelete() {
	if (mpShape) {
		ShapeAABB* rect = static_cast<ShapeAABB*>(mpShape);
		rect->mTop = 0.0f;
		rect->mBottom = 0.0f;
		rect->mLeft = 0.0f;
		rect->mRight = 0.0f;
	}
	mpTr = nullptr;
	mpBody = nullptr;
	isTrigger = false;
	gpComponentManager->mCollider->Delete(this);
}

void Collider::Clone(Component * rhs1) {

	Collider* rhs = static_cast<Collider*>(rhs1);
	if (rhs->mpShape->mType == AABB) {
		mpShape = gpComponentManager->mShapeAABB->Add(mpOwner->id);
		mpShape->mpOwnerCollider = this;
		//this->mpShape = new ShapeAABB();

	}
	// TODO make generic for Spheres and AABBs

	/*TODO Right now is not possible to clone
	until we  secuence creation of compoents
	because for determining the collider have a "Model" component
	*/

	rhs->Init();



	//mpShape = rhs->mpShape;
	coeffRestitution = rhs->coeffRestitution;
	coeffStaticFriction = rhs->coeffStaticFriction;
	coeffDynamicFriction = rhs->coeffDynamicFriction;
	isTrigger = rhs->isTrigger;
	//Transform* mpTr;
	//Body* mpBody;
}