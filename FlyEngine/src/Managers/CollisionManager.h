/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#pragma once

#include <list>
#include "../Components/Collider.h"
#include "../Components/Body.h"
#include "../Components/Shape.h"


class Contact
{
public:
	Contact(Collider*, Collider*);
		
	Collider* mpCollider[2];
	glm::vec2 colNormal;
	float penetrationDepth;
	glm::vec3 colPoint;

	float e;
	float cSF;
	float cDF;
//private:
	Body* A;
	Body* B;

public:
	void ApplyImpulse() {

		glm::vec2 relativeVel = B->mVel - A->mVel;
		float velNormal = glm::dot(relativeVel, colNormal);

		if (velNormal > 0) return; // means the objects are separating


		// linear impulse
		// TODO upgrade to angular impulse
		float impulseMagnitude = -(1.0f + e) * velNormal / (A->mInvMass + B->mInvMass);
		glm::vec2 impulseVec = impulseMagnitude * colNormal;

			A->mVel -= impulseVec * A->mInvMass;
			B->mVel += impulseVec * B->mInvMass;


		// TODO implement friction with more contact points
		// having one contact point makes it unstable and unusable
			if(A->applyFriction && B->applyFriction)
				ApplyFrictionImpulse(impulseMagnitude);
	}

	void ApplyFrictionImpulse(float impulse) {
		glm::vec2 relativeVel = B->mVel - A->mVel;

		float relativeVelNormal = glm::dot(relativeVel, colNormal);
		glm::vec2 tangent = relativeVel - (colNormal * relativeVelNormal);
		tangent = glm::normalize(tangent);
		
		// check if tangent is nan
		if (isnan(tangent.x) || (isnan(tangent.y))) {
			return;
		}

		//std::cout << "tangent : x " << tangent.x << "tangent : y " << tangent.y << std::endl;

		float fricImpulseMagnitude = -glm::dot(relativeVel, tangent) / (A->mInvMass + B->mInvMass);
		
		// not applying zero impulse
		if (fricImpulseMagnitude == 0.0f) return;

		glm::vec2 frictionImpulse;
		
		// Coulomb's law of friction
		if(abs(fricImpulseMagnitude) < impulse * cSF)
			frictionImpulse = fricImpulseMagnitude * tangent;
		else
			frictionImpulse = -impulse * tangent * cDF;

		A->mVel -= frictionImpulse * A->mInvMass;
		B->mVel += frictionImpulse * B->mInvMass;

	}

	void CorrectPenetration() {
		float slop = 0.01f; // if penetration is greater than this only then correct it
		float percent = 0.4f; // resolving 40% of the actual penetration depth
		glm::vec2 correction = (std::max(penetrationDepth - slop, 0.0f) / (A->mInvMass + B->mInvMass)) * percent * colNormal;
		A->mPos -= correction * A->mInvMass;
		B->mPos += correction * B->mInvMass;
	}
};

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	
	void Reset();
	bool CheckCollisionAndGenerateContact(Shape *pShape1, glm::vec2 Pos1,
		Shape *pShape2, glm::vec2 Pos2);
public:
	std::list<Contact*> mContacts;

private:
	bool (*CollisionFunctions[ShapeType::SHAPESNUM][ShapeType::SHAPESNUM])(Shape *pShape1, glm::vec2 Pos1,
		Shape *pShape2, glm::vec2 Pos2, std::list<Contact*> &mContacts);
};


