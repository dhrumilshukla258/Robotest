/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Ivan Cazares, ivan.c, 60004418
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "OscillationSystem.h"
#include "../Entity.h"
#include "../Managers/ComponentManager.h"

extern ComponentManager* gpComponentManager;

OscillationSystem::OscillationSystem()
{
}

OscillationSystem::~OscillationSystem()
{
}

// Platform  defines  X and Y offsets, 
/*UPDATE
-  can start moving to left or down
- if collides with other element before reaching limits, (vel==0) then reverses to the opposite limit
- never forgets its own vel
- doesnt need booleans to indicate direction, it is in velocity
*/

/*Changes:... 
- change tr for Reset, 
- no need to use  LeftRight
- 				UpDown
*/
void OscillationSystem::Update() 
{
	Component* c = gpComponentManager->mOscillation->GetHead();
	while (c != nullptr) {
		Oscillation* oSci = static_cast<Oscillation*>(c);
		Body* pBody = oSci->mpOwner->GetComponent<Body>();

		if (oSci->mStart) 
		{
			if (oSci->mReset) {
				oSci->Init();
				pBody->mVel = oSci->mVelocity;
				oSci->mReset = false;
				break;// permits write pBodyPrevPosition - allowing start on left or down
			}
			//-------------VELX-------------------
			oSci->idealVel.x =
			(oSci->mVelocity.x == 0.0f || oSci->mLeftRightDistance == 0) ?// any requirement on 0
					0:
			pBody->mPos.x > oSci->UPos.x ?//touches right, go left
					-fabs(oSci->mVelocity.x) :

			pBody->mPos.x < oSci->LPos.x ?//touches left, go right
					fabs(oSci->mVelocity.x) :

			pBody->mPos.x == pBody->mPrevPos.x ?//stuck ? reverse
					-oSci->idealVel.x :

			oSci->idealVel.x;

			//-------------VELY-------------------
			oSci->idealVel.y =
			(oSci->mVelocity.y == 0.0f || oSci->mUpDownDistance == 0)?// any requirement on 0
						0:
			pBody->mPos.y > oSci->UPos.y ?//touches down go up
				-fabs(oSci->mVelocity.y) :

			pBody->mPos.y < oSci->LPos.y ?//touches up, go down
				fabs(oSci->mVelocity.y) :

			pBody->mPos.y == pBody->mPrevPos.y ?//stuck ? reverse
				-oSci->idealVel.y :
			
				oSci->idealVel.y;
			
			pBody->mVel = oSci->idealVel;


			pBody->pTr->mDirtyFlag = true;
		}
		c = c->GetNext();
	}
}