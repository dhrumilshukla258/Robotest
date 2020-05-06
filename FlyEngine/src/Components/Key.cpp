/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Key.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;
extern bool gEditMode;

RTTR_REGISTRATION
{
	rttr::registration::class_<Key>("Key")
		 .property("EntityVec", &Key::mEntity);
}

Key::Key()
{
	mComponentName = "Key";
}


Key::~Key()
{
}

void Key::Init()
{
	Transform* pTr = mpOwner->GetComponent<Transform>();
	unsigned int i = 1;

	for (auto entity : mEntity) {
		entity->mOwnerEntity = mpOwner;
		mpOwner->mChildEntities.push_back(entity);

		if (gEditMode) {
			entity->GetComponent<Transform>()->mPos = pTr->mPos + pTr->mScale / 2.0f + glm::vec3((float)(5 * i), 0.0f, 0.0f);
			++i;
		}
	}
}

void Key::DeleteChildReference(unsigned int id)
{
	for(auto entity : mEntity)
		if (entity->id == id) {
			entity = nullptr;
	}
}

void Key::SelfDelete() {
	mEntity.clear();
	gpComponentManager->mKey->Delete(this);
}

void Key::Clone(Component * rhs1) {
	Key* rhs = static_cast<Key*>(rhs1);
}