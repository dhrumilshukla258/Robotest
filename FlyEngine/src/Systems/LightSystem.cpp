/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/


#include "../pch.h"
#include "LightSystem.h"
#include "../Components/World.h"
#include "../Components/Light.h"
#include "../Components/Transform.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/SystemManager.h"
#include "PlayerSystem.h"
#include "../Entity.h"
#include "../Managers/CameraManager.h"

extern CameraManager* gpCameraManager;

#define  Player_Light_Distance 25000.0f
extern ComponentManager* gpComponentManager;
extern 	SystemManager*   gpSystemManager;
LightSystem::LightSystem()
{
}

LightSystem::~LightSystem()
{
}

float Disteance_Check(glm::vec3 A, glm::vec3 B)
{
	return fabs((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y) + (A.z - B.z)*(A.z - B.z));
}

void LightSystem::Update()
{

	World* pWorld;
	Light* pLight;
	Transform *pTr;
	
	Model* pMd;
	
	PlayerSystem* mPlayer_system_temp = gpSystemManager->mPlayerSystem;
	Component* c_world = gpComponentManager->mWorld->GetHead();
	pWorld = static_cast<World*>(c_world);
	if (pWorld == nullptr)
	{
		return;
	}
	pWorld->light_list.clear();

	/*
	if (mPlayer_system_temp->GetPlayerEntity() == nullptr)
	{
		Component* c = gpComponentManager->mLight->GetHead();
		float dist = 111110.0f;

		while (nullptr != c)
		{
			pLight = static_cast<Light*>(c);

			pTr = pLight->mpOwner->GetComponent<Transform>();
			pMd = pLight->mpOwner->GetComponent<Model>();
			pLight->point_Light_Position = pTr->mPos;
			
			pMd->light = true;
			pWorld->light_list.push_back(pLight);

			c = c->GetNext();
		}
		return;
	}
	*/
	
	
	Component* c = gpComponentManager->mLight->GetHead();
	//float dist=111110.0f;

	while (nullptr != c) 
	{
			pLight = static_cast<Light*>(c);
			
			pTr = pLight->mpOwner->GetComponent<Transform>();
			pMd = pLight->mpOwner->GetComponent<Model>();
			pLight->point_Light_Position = pTr->mPos;
			//pMd->mCol = pLight->diffuse;
			///pWorld->light_list.push_back(pLight);
			//pWorld->light_list.push_back(pLight);
		
			/*
			if (dist > Disteance_Check(gpCameraManager->mPosition, pTr->mPos))
			{
				pWorld->light_Closest = pLight;
				dist = Disteance_Check(gpCameraManager->mPosition, pTr->mPos);
			}*/
			
			if (Disteance_Check(gpCameraManager->mPosition, pTr->mPos) < Player_Light_Distance)
			{
				pMd->light = true;
				pWorld->light_list.push_back(pLight);
			}
			else
			{
				pMd->light = false;
				//pMd->mCol = glm::vec3(1.0f,1.0f,1.0f);
			}
			

			//pMd->light = true;
			//pWorld->light_list.push_back(pLight);

			c = c->GetNext();
	}

}