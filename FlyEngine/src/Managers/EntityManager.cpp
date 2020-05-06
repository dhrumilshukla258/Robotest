/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "EntityManager.h"

#include "ComponentManager.h"

extern ComponentManager * gpComponentManager;

EntityManager::EntityManager()
{
	block = new Entity[1024]; 
	for (int i = 0; i < 1024; ++i) {
		mInactive.push_front(&block[i]);
	}
}

EntityManager::~EntityManager()
{
	delete[] block;
	mInactive.clear();
	mEntity.clear();
}

Entity* EntityManager::CreateEntity() {
	mEntity[id] = mInactive.front();
	mInactive.pop_front();
	mEntity[id]->mComponents.clear();
	mEntity[id]->id = id;
	++id;
	return mEntity[id-1];
}

void EntityManager::DeleteEntity(unsigned int _id) {
	if (mEntity.find(_id) != mEntity.end()) {
		Entity* e = mEntity[_id];
		int size = e->mComponents.size();
		
		if (e->mChild != nullptr) {
			Transform* childpTr = e->mChild->GetComponent<Transform>();
			Body* childpBody = e->mChild->GetComponent<Body>();
			Transform* pTr = e->mChild->mParent->GetComponent<Transform>();
			childpTr->mPos = pTr->mPos + childpTr->mChildOffset;
			
			childpBody->mPos.x = childpTr->mPos.x;
			childpBody->mPos.y = childpTr->mPos.y;
			childpBody->useGravity = true;
			e->mChild->mParent = nullptr;
			e->mChild = nullptr;
		}

		if (e->mParent != nullptr) {
			e->mParent->mChild = nullptr;
			e->mParent = nullptr;
		}

		if (e->mChildEntities.size() != 0) {
			for (auto mChildEntity : e->mChildEntities) {
				mChildEntity->mOwnerEntity = nullptr;
				DeleteEntity(mChildEntity->id);
			}
			e->mChildEntities.clear();
		}
		
		if (e->mOwnerEntity) {
			for (auto ownerComponent : e->mOwnerEntity->mComponents) {
				ownerComponent->DeleteChildReference(e->id);
			}
			e->mOwnerEntity = nullptr;
			for (int i = 0; i < size; ++i) {
				e->mComponents[i]->SelfDelete();
			}
		}
		else {
			for (int i = 0; i < size; ++i) {
				e->mComponents[i]->SelfDelete();
			}
		}
		
		gpComponentManager->invoke("DeleteComponent", _id);
		mInactive.push_front(mEntity[_id]);
		mEntity.erase(_id);
	}
}

// FOR EDITOR
void EntityManager::AddToEditorBuffer(unsigned int _id)
{
	if (mEntity[_id]) {
		Entity* pEntity = mEntity[_id];
		
		if (pEntity->mChildEntities.size() != 0) {
			for (auto childEntity : pEntity->mChildEntities) {
				AddToEditorBuffer(childEntity->id);
			}
		}

		int size = pEntity->mComponents.size();
		for (int i = 0; i < size; ++i) {
			pEntity->mComponents[i]->SelfBuffer();
		}
		mEditorBufferEntities[pEntity->id] = pEntity;
		mEntity.erase(pEntity->id);
	}
}

void EntityManager::RemoveFromEditorBuffer(unsigned int _id)
{
	if (mEditorBufferEntities[_id]) {
		Entity* pEntity = mEditorBufferEntities[_id];
		mEditorBufferEntities.erase(_id);

		if (pEntity->mChildEntities.size() != 0) {
			for (auto childEntity : pEntity->mChildEntities) {
				RemoveFromEditorBuffer(childEntity->id);
			}
		}

		int size = pEntity->mComponents.size();
		for (int i = 0; i < size; ++i) {
			pEntity->mComponents[i]->SelfBufferRemove();
		}
		mEntity[pEntity->id] = pEntity;
	}
}

void EntityManager::ClearEditorBuffer()
{
	for (auto c : mEditorBufferEntities) {
		Entity* pEntity = mEditorBufferEntities[c.second->id];

		if (pEntity->mOwnerEntity) {
			for (auto ownerComponent : pEntity->mOwnerEntity->mComponents) {
				ownerComponent->DeleteChildReference(pEntity->id);
			}
			pEntity->mOwnerEntity = nullptr;
		}

		int size = pEntity->mComponents.size();
		for (int i = 0; i < size; ++i) {
			pEntity->mComponents[i]->SelfBufferClear();
		}
	}
	mEditorBufferEntities.clear();
}
// -----------------