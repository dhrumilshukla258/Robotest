/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "Text.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/ObjectFactory.h"
extern ComponentManager* gpComponentManager;
extern ObjectFactory* gpObjectFactory;

RTTR_REGISTRATION
{
	rttr::registration::class_<Text>("Text")
		 .property("Str", &Text::mStr)
		 .property("JoyStickStr", &Text::mJoyStickStr)
		 .property("TextTrgPT", &Text::mTextCollider)
		 .property("TextAppear", &Text::mTextAppear)
		 .property("TextColor", &Text::mTextColor);
}

Text::Text() : mTextCollider(nullptr), mTextAppear(false)
{
	mComponentName = "Text";
}


Text::~Text()
{
}

void Text::Init()
{
	if (mTextCollider) {
		mTextCollider->mOwnerEntity = mpOwner;
		mpOwner->mChildEntities.push_back(mTextCollider);
	}
}

void Text::DeleteChildReference(unsigned int id)
{
	if (mTextCollider != nullptr)
		if (mTextCollider->id == id) {
			mTextCollider = nullptr;
		}
}

void Text::SelfReset()
{
	mTextCollider = nullptr;
	mTextAppear = false;
	mStr = "";
	mJoyStickStr = "";
	mTextColor = glm::vec3(0.0f);
}

void Text::Clone(Text *rhs)
{
}

void Text::SelfDelete()
{
	SelfReset();
	gpComponentManager->mText->Delete(this);
}