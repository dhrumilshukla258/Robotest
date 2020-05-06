/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Script.h"

#include "../InitData.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<Script>("Script")
		 .property("Lua_File", &Script::mLuaFile)
		 .property("x", &Script::x);
}

Script::Script() : onEachFrame(false)
{
	mComponentName = "Script";
}

Script::~Script()
{
}

void Script::Init()
{
	lua[std::to_string(mpOwner->id)] = lua.create_table_with("x", x, "y", 4);
	// creates a table in Lua State:
	/*
		"id" : {
			"x" : 0.05,
			"y" : 4
		}
	*/
}

void Script::SelfDelete()
{
	mLuaFile = "";
	gpComponentManager->mScript->Delete(this);
}

void Script::Clone(Component * rhs1) {
	Script* rhs = static_cast<Script*>(rhs1);
	mLuaFile = rhs->mLuaFile;
	onEachFrame = rhs->onEachFrame;
	x = rhs->x;
}
