/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "ObjectFactory.h"

#include "ResourceManager.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "../Systems/PlayerSystem.h"
#include "../Utilities/Reflection.h"

extern ResourceManager* gpResourceManager;
extern EntityManager* gpEntityManager;
extern ComponentManager* gpComponentManager;
extern SystemManager* gpSystemManager;
extern Reflection* gpReflection;

ObjectFactory::ObjectFactory()
{
}


ObjectFactory::~ObjectFactory()
{
}

//void ObjectFactory::LoadLevel(Document* _pDoc)
//{
//	GenericObject<false, Value::ValueType> doc = _pDoc->GetObject();
//
//	for (Value::MemberIterator lvlItr = doc.MemberBegin(); lvlItr != doc.MemberEnd(); ++lvlItr)
//	{
//		//Here if the lvlItr is not object we can also use it as a string and 
//		//complete use it that way.
//		if (lvlItr->value.IsObject()) {
//
//			GenericObject<false, rapidjson::Value::ValueType> objDoc = lvlItr->value.GetObject();
//			Entity* pNewEntity = nullptr;
//
//			if (objDoc.HasMember("file")) {
//				pNewEntity = LoadObject(objDoc["file"].GetString());
//				if (strcmp(objDoc["file"].GetString(), "Player1.json") == 0)
//					gpSystemManager->mPlayerSystem->SetPlayerEntity(pNewEntity);
//			}
//
//			if (pNewEntity) {
//				if (objDoc.HasMember("Transform")) {
//					Body * pB = nullptr;
//					Model* pMd = nullptr;
//					Transform* pTr = nullptr;
//					
//					auto pTr_itr = gpComponentManager->mTransform->mActive.find(pNewEntity->id);
//					pTr = static_cast<Transform*>(pTr_itr->second);
//					//pTr->Serialize(objDoc["Transform"].GetObject());
//					
//					auto pB_itr = gpComponentManager->mBody->mActive.find(pNewEntity->id);
//					if (pB_itr != gpComponentManager->mBody->mActive.end()) {
//						pB = static_cast<Body*>(pB_itr->second);
//						pB->Initialize();
//					}
//					
//					auto pMd_itr = gpComponentManager->mModel->mActive.find(pNewEntity->id);
//					if (pMd_itr != gpComponentManager->mModel->mActive.end()) {
//						pMd = static_cast<Model*>(pMd_itr->second);
//						// TODO make generic for Spheres and AABBs
//						ShapeAABB* rect = static_cast<ShapeAABB*>(pB->mpShape);
//						rect->mTop = pMd->y_max * pB->pTr->mScale.y;
//						rect->mBottom = pMd->y_min * pB->pTr->mScale.y;
//						rect->mLeft = pMd->x_min * pB->pTr->mScale.x;
//						rect->mRight = pMd->x_max * pB->pTr->mScale.x;
//					}
//						
//				}
//				// .........
//			}
//			
//
//		}
//
//
//	}
//}

//Entity* ObjectFactory::LoadObject(std::string _filename)
//{
//	Entity* pEntity = gpEntityManager->CreateEntity();
//
//	Document* pDoc = gpResourceManager->LoadPrefab(_filename);
//	if (!pDoc) assert("JSON not found for object file");
//	
//	GenericObject<false, Value::ValueType> doc = pDoc->GetObject();
//
//	if (doc.HasMember("Transform")) {
//		Transform* pTr = static_cast<Transform*>(gpComponentManager->mTransform->Add(pEntity->id));
//		//pTr->Serialize(doc["Transform"].GetObject());
//	}
//
//	if (doc.HasMember("Sprite")) {
//		Sprite* pSp = static_cast<Sprite*>(gpComponentManager->mSprite->Add(pEntity->id));
//		//pSp->Serialize(doc["Sprite"].GetObject());
//	}
//
//	if (doc.HasMember("Model")) {
//		Model* pMd = static_cast<Model*>(gpComponentManager->mModel->Add(pEntity->id));
//		//pMd->Serialize(doc["Model"].GetObject());
//	}
//
//	if (doc.HasMember("Body")) {
//		Body* pB = static_cast<Body*>(gpComponentManager->mBody->Add(pEntity->id));
//		pB->pTr = static_cast<Transform*>(gpComponentManager->mTransform->mActive[pEntity->id]);
//		//pB->Serialize(doc["Body"].GetObject());
//	}
//
//	return pEntity;
//}


Entity* ObjectFactory::CreateRuntimeObject(std::string _filename)
{
	Entity* en = gpReflection->LoadObject(_filename);
	//en->ResetComponent();
	for (auto comp : en->mComponents) {
		comp->Init();
	}
	return en;
}


void ObjectFactory::DeleteObject(unsigned int _id)
{
	mDeletedObjs.push_back(_id);
}

void ObjectFactory::Update()
{
	for (unsigned int i = 0; i < mDeletedObjs.size(); ++i) {
		gpEntityManager->DeleteEntity(mDeletedObjs[i]);
	}
	mDeletedObjs.clear();
}