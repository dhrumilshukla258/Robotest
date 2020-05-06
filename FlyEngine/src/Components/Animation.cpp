/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Animation.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/GraphicsManager.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<Animation>("Animation")
		.constructor()
		.property("current_animation", &Animation::mCurrent_animation)
		.property("previous_animation", &Animation::mPrevious_animation)
		.property("start_Time", &Animation::mStart_Time)
		.property("stop_Time", &Animation::mStop_Time)
		.property("animation_Speed", &Animation::mAnimation_Speed)
		.property("animation_Repeat", &Animation::mAnimation_Repeat);
}

extern ResourceManager* gpResourceManager;
extern GraphicsManager* gpGraphicsManager;

Animation::Animation() :
	mCurrent_animation(0),
	mPrevious_animation(0),
	mStart_Time(0.0f),
	mAnimation_Speed(0.0f), 
	mAnimation_Repeat(false),
	mAnimation_Stop(false),
	mAnimation_Time(0.0f)
{
	mpOwner = nullptr;
	mComponentName = "Animation";
}

Animation::~Animation()
{
}

void Animation::Init()
{
	if (!mpOwner) {
		mCurrent_animation = 0;
		mPrevious_animation = 0;
		mStart_Time = 0.0f;
		mStop_Time = 0.0f;
		mAnimation_Speed = 0.0f;
		mAnimation_Repeat = false;
		mAnimation_Stop = false;
		//list<int>  mAnimation_Stack;
		mAnimation_Time = 0.0f;
	}
}

void Animation::SelfDelete() {
	mpOwner = nullptr;
	Init();
	mAnimation_Stack.clear();
	gpComponentManager->mAnimation->Delete(this);
}

void Animation::Clone(Component * rhs1) {
	Animation* rhs = static_cast<Animation*>(rhs1);
	mCurrent_animation = rhs->mCurrent_animation;
	mPrevious_animation = rhs->mPrevious_animation;
	mStart_Time = rhs->mStart_Time;
	mStop_Time = rhs->mStop_Time;
	mAnimation_Speed = rhs->mAnimation_Speed;
	mAnimation_Repeat = rhs->mAnimation_Repeat;
	mAnimation_Stop = rhs->mAnimation_Stop;
	//list<int>  mAnimation_Stack;
	mAnimation_Time = rhs->mAnimation_Time;
}