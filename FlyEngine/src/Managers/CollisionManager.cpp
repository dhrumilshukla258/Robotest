/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "CollisionManager.h"
#include "../Entity.h"
#include "../Components/Shape.h"
#include "../Components/Collider.h"
#include "../Components/Transform.h"
#include "SystemManager.h"
#include "../Systems/PhysicsSystem.h"

extern SystemManager* gpSystemManager;

#define EPSILON 0.000001

#include <glm/gtx/norm.hpp>
Contact::Contact(Collider* col0, Collider* col1) {
	mpCollider[0] = col0;
	mpCollider[1] = col1;

	A = mpCollider[0]->mpBody;
	B = mpCollider[1]->mpBody;

	e = std::min(mpCollider[0]->coeffRestitution, mpCollider[1]->coeffRestitution);
	cSF = (mpCollider[0]->coeffStaticFriction + mpCollider[1]->coeffStaticFriction) / 2;
	cDF = (mpCollider[0]->coeffDynamicFriction + mpCollider[1]->coeffDynamicFriction) / 2;

	penetrationDepth = 0.0f;
	colNormal = glm::vec2(0.0f, 0.0f);

	// resting collision
	if(A && B)
	if (length2(B->mVel - A->mVel) < length2(gpSystemManager->mPhysicsSystem->fixedDeltaTime * gpSystemManager->mPhysicsSystem->gravity) + EPSILON)
		e = 0.0f;
}

bool CheckCollisionCircleCircle(Shape *pShape1, glm::vec2 Pos1,
	Shape *pShape2, glm::vec2 Pos2, std::list<Contact*> &Contacts)
{
	ShapeCircle* c1 = static_cast<ShapeCircle*>(pShape1);
	ShapeCircle* c2 = static_cast<ShapeCircle*>(pShape2);

	if (glm::distance(Pos1, Pos2) <= (c1->mRadius + c2->mRadius)) {
		Contact *pNewContact = new  Contact(pShape1->mpOwnerCollider, pShape2->mpOwnerCollider);

		Contacts.push_back(pNewContact);

		return true;
	}

	return false;
};

// TODO recheck this, works for A->B not B->A
glm::vec2 CheckDirection(glm::vec2 diff) {
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};

	diff = glm::normalize(diff); // sqRoot (Careful!)

	float max = 0.0f;
	int best_match = -1;
	for (int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(diff, compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}

	return compass[best_match];
}

bool CheckCollisionAABBCircle(Shape *pShape1, glm::vec2 Pos1,
	Shape *pShape2, glm::vec2 Pos2, std::list<Contact*> &Contacts)
{

	ShapeAABB *r1 = static_cast<ShapeAABB*>(pShape1);
	ShapeCircle* c2 = static_cast<ShapeCircle*>(pShape2);

	float width = r1->mRight - r1->mLeft + 2 * c2->mRadius;
	float height = r1->mTop - r1->mBottom + 2 * c2->mRadius;

	if ((Pos2.x < (Pos1.x - width / 2)) ||
		(Pos2.x > (Pos1.x + width / 2)) ||
		(Pos2.y < (Pos1.y - height / 2)) ||
		(Pos2.y > (Pos1.y + height / 2))) {

		return false;
	} else {
		// the positions inside this if are of the collision
		// we can use these to find out position of collision
		// to better reflect off the paddle
		glm::vec2 centerDistance = Pos2 - Pos1;

		//clamping center distance gives us the vector to the nearest side of the AABB
		centerDistance.x = (centerDistance.x > r1->GetHalfExtents().x) ? r1->GetHalfExtents().x : centerDistance.x;
		centerDistance.x = (centerDistance.x < -r1->GetHalfExtents().x) ? -r1->GetHalfExtents().x : centerDistance.x;
		centerDistance.y = (centerDistance.y > r1->GetHalfExtents().y) ? r1->GetHalfExtents().y : centerDistance.y;
		centerDistance.y = (centerDistance.y < -r1->GetHalfExtents().y) ? -r1->GetHalfExtents().y : centerDistance.y;

		// vector to closest point on AABB
		glm::vec2 closest = Pos1 + centerDistance;

		// vector from circle center to closest point
		glm::vec2 difference = closest - Pos2;

		Contact *pNewContact = new  Contact(pShape1->mpOwnerCollider, pShape2->mpOwnerCollider);
		pNewContact->penetrationDepth = std::max(difference.x, difference.y);
		pNewContact->colNormal = CheckDirection(difference);

		Contacts.push_back(pNewContact);

		return true;
	}
	return false;
}

bool CheckCollisionCircleAABB(Shape *pShape1, glm::vec2 Pos1,
	Shape *pShape2, glm::vec2 Pos2, std::list<Contact*> &Contacts)
{
	return CheckCollisionAABBCircle(pShape2, Pos2, pShape1, Pos1, Contacts);
}

bool CheckCollisionAABBAABB(Shape *pShape1, glm::vec2 Pos1,
	Shape *pShape2, glm::vec2 Pos2, std::list<Contact*> &Contacts)
{
	ShapeAABB *r1 = static_cast<ShapeAABB*>(pShape1);
	ShapeAABB *r2 = static_cast<ShapeAABB*>(pShape2);

	glm::vec2 r1extents = r1->GetHalfExtents();
	glm::vec2 r2extents = r2->GetHalfExtents();

	glm::vec2 separationNow = Pos2 - Pos1;

	float xSeparationNow = r1extents.x + r2extents.x - abs(separationNow.x);
	if(xSeparationNow > 0) {
		float ySeparationNow = r1extents.y + r2extents.y - abs(separationNow.y);

		if (ySeparationNow > 0) {
			// Generate Contact and set data for reflection
			Contact *pNewContact = new  Contact(pShape1->mpOwnerCollider, pShape2->mpOwnerCollider);

			if (xSeparationNow < ySeparationNow) {
				if (separationNow.x < 0) {
					pNewContact->colNormal = glm::vec2(-1, 0);
				}
				else {
					pNewContact->colNormal = glm::vec2(1, 0);
				}
				pNewContact->penetrationDepth = xSeparationNow;
			}
			else {
				if (separationNow.y < 0) {
					pNewContact->colNormal = glm::vec2(0, -1);
				}
				else {
					pNewContact->colNormal = glm::vec2(0, 1);
				}
				pNewContact->penetrationDepth = ySeparationNow;
			}
			
			pNewContact->colPoint = glm::vec3(
				pShape1->mpOwnerCollider->mpTr->mPos.x + r1extents.x -
					pNewContact->colNormal.x * pNewContact->penetrationDepth,
				pShape1->mpOwnerCollider->mpTr->mPos.y + r1extents.y - 
					pNewContact->colNormal.y * pNewContact->penetrationDepth,
				0.0f);

			Contacts.push_back(pNewContact);

			return true;
		}
	}

	return false;
}

CollisionManager::CollisionManager()
{
	// set all function pointers
	CollisionFunctions[CIRCLE][CIRCLE] = CheckCollisionCircleCircle;
	CollisionFunctions[CIRCLE][AABB] = CheckCollisionCircleAABB;
	CollisionFunctions[AABB][CIRCLE] = CheckCollisionAABBCircle;
	CollisionFunctions[AABB][AABB] = CheckCollisionAABBAABB;
}

CollisionManager::~CollisionManager()
{
	Reset();
}

void CollisionManager::Reset()
{
	for (auto contact : mContacts) {
		delete contact;
	}

	mContacts.clear();
}

bool CollisionManager::CheckCollisionAndGenerateContact(Shape * pShape1, glm::vec2 Pos1, Shape * pShape2, glm::vec2 Pos2)
{
	return CollisionFunctions[pShape1->mType][pShape2->mType](pShape1, Pos1, pShape2, Pos2, mContacts);
}