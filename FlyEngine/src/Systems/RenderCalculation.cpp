/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/



#include "../pch.h"
#include "RenderCalculation.h"
#include "../Managers/ComponentManager.h"
#include "../Components/Shape.h"

extern ComponentManager* gpComponentManager;

RenderCalculation::RenderCalculation()
{
}

RenderCalculation::~RenderCalculation()
{
}

void RenderCalculation::CalculateModelingMatrix() {
	Component* c = gpComponentManager->mModel->GetHead();
	while (c != nullptr) {

		Model* pMd = static_cast<Model*>(c);
		Transform* pTr = pMd->mpOwner->GetComponent<Transform>();

		if (nullptr != pTr) {

			//if (pTr->mDirtyFlag)
		//	{

				if (pMd->mpOwner->mParent != nullptr) {
					pTr->mPos = pTr->mpOwner->mParent->GetComponent<Transform>()->mPos;
					pTr->mPos += pTr->mChildOffset;
				}

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, pTr->mPos);
				model = glm::rotate(model, glm::radians(pTr->mRotate), pTr->mRotationDir);
				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(pTr->mScale));
				*(pMd->mModelMatrix) = model;

				
		//	}

				pTr->mDirtyFlag = false;
			
		}
		c = c->GetNext();
	}
}

void RenderCalculation::Calculate_Stencil_Toon_Modeling_Matrix()
{

	Component* c = gpComponentManager->mStencil_Toon_Model->GetHead();
	while (c != nullptr) {

		Stencil_Toon_Model* pSTMd = static_cast<Stencil_Toon_Model*>(c);
		Transform* pTr = pSTMd->mpOwner->GetComponent<Transform>();

		if (nullptr != pTr) {

		//	if (pTr->mDirtyFlag)
		//	{

				if (pSTMd->mpOwner->mParent != nullptr) {
					pTr->mPos = pTr->mpOwner->mParent->GetComponent<Transform>()->mPos;
					pTr->mPos += pTr->mChildOffset;
				}

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, pTr->mPos);
				model = glm::rotate(model, glm::radians(pTr->mRotate), pTr->mRotationDir);
				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(pTr->mScale));
				*(pSTMd->mModelMatrix) = model;

				glm::mat4 model_Stencil = glm::mat4(1.0f);
				model_Stencil = glm::translate(model_Stencil, pTr->mPos);
				model_Stencil = glm::rotate(model_Stencil, glm::radians(pTr->mRotate), glm::vec3(0, 1, 0.0f));
				model_Stencil = glm::rotate(model_Stencil, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				
				model_Stencil = glm::scale(model_Stencil, glm::vec3(pTr->mScale*1.05f));
				*(pSTMd->mStencil_Matrix) = model_Stencil;
	//		}

			pTr->mDirtyFlag = false;

		}
		c = c->GetNext();
	}
}

void RenderCalculation::Update() 
{
	
	CalculateModelingMatrix();
	Calculate_Stencil_Toon_Modeling_Matrix();

	/*Component* cb = gpComponentManager->mBackground_Model->GetHead();
	while (cb != nullptr) {

		BackgroundModel* pBMd = static_cast<BackgroundModel*>(cb);
		Transform* pTr = pBMd->mpOwner->GetComponent<Transform>();

		if (nullptr != pTr) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pTr->mPos);
			model = glm::rotate(model, glm::radians(pTr->mRotate), glm::vec3(0, 1, 0.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(pTr->mScale));
			*(pBMd->mBackground_ModelMatrix) = model;
			
			
			
		}
		cb = cb->GetNext();
	}*/

	Component* co = gpComponentManager->mCollider->GetHead();
	while (co != nullptr)
	{
		Collider* pCol = static_cast<Collider*>(co);
		Transform* pTr = pCol->mpOwner->GetComponent<Transform>();
		if (nullptr != pTr) {
			glm::vec2 extents = pCol->mpShape->GetHalfExtents();

			glm::mat4 model_Debug = glm::mat4(1.0f);
			glm::vec3 temp_ptr(pTr->mPos.x, pTr->mPos.y, pTr->mPos.z);
			model_Debug = glm::translate(model_Debug, temp_ptr);
			model_Debug = glm::rotate(model_Debug, glm::radians(pTr->mRotate), glm::vec3(0, 1, 0.0f));
			glm::vec3 temp_scl((extents.x * 2), (extents.y * 2), 0.1f);
			model_Debug = glm::scale(model_Debug, glm::vec3(temp_scl));

			*(pCol->mDebugMatrix) = model_Debug;
		}
		co = co->GetNext();
	}
	
}
