/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "World.h"

#include "../Managers/ComponentManager.h"

extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<World>("World");
}

World::World() : light_list(), light_Closest(nullptr)
{
	mComponentName = "World";
}

World::~World()
{
}

void World::Init()
{
}

void World::SelfDelete()
{
	light_list.clear();
	gpComponentManager->mWorld->Delete(this);
}
