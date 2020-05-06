/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/


#include "../pch.h"

#include "ScriptingSystem.h"

#include <dirent.h>
#include "../InitData.h"
#include "../Components/Script.h"
#include "../Managers/ComponentManager.h"
#include "../Components/Body.h"

extern ComponentManager* gpComponentManager;

//extern sol::state gLuaState;
std::unordered_map<std::string, sol::load_result> mScripts;

ScriptingSystem::ScriptingSystem() {
	lua.open_libraries(sol::lib::base);

	LoadAllScripts();
	
	sol::protected_function fx = lua["sentinelVelocity"];
	if (fx.valid()) {
		sentinelVelocity = fx;
	}

	lua.new_usertype<Body>("Body", "Move", sol::as_function(&Body::Move));
}

ScriptingSystem::~ScriptingSystem() {
	
}

void ScriptingSystem::LoadAllScripts()
{
	DIR *pDIR;
	struct dirent *entry;
	if (pDIR = opendir(directory::script.c_str())) {
		while (entry = readdir(pDIR)) {
			std::string script_name = entry->d_name;
			if (script_name.find(".lua") != std::string::npos) {
				
				sol::load_result script = lua.load_file(directory::script + script_name);
				// EXECUTE SCRIPT ONCE
				sol::protected_function_result scriptResult = script();
				
				if (!scriptResult.valid()) {
					sol::error err = scriptResult;
					std::string what = err.what();
					LOG_INFO("FlyEngine", script_name + " Script result invalid." + what);
				}
				else {
					mScripts[script_name] = script;
				}
			}
		}
		closedir(pDIR);
	}
}

void ScriptingSystem::Update(float deltaTime) {
	Component* c = gpComponentManager->mScript->GetHead();
	while (nullptr != c) {
		Script* pScript = static_cast<Script*>(c);
		std::string lua_file = pScript->mLuaFile;

		if (pScript->onEachFrame) {
			if (mScripts.find(lua_file) != mScripts.end()) {
				sol::load_result& script = mScripts[lua_file];
				sol::protected_function_result scriptresult = script();
				//sol::protected_function_result scriptresult = mScripts[lua_file]();  // ALSO VALID
			}
		}

		// TEMPORARY ----------------------------------------------------------
			
		// C++ fn from Lua : Move.lua file  -----------------------------------------------------------------
		if ("Move.lua" == lua_file) {
			Body* pBody = pScript->mpOwner->GetComponent<Body>();
			lua.set("obj", pBody);

			sol::protected_function_result scriptResult = lua.script_file(directory::script + lua_file); // load & run lua file
			if (!scriptResult.valid()) {
				sol::error err = scriptResult;
				std::string what = err.what();
				LOG_INFO("FlyEngine", lua_file + ". Script result invalid: " + what);
			}
		}

		// CODE THAT IS NOT WORKING
		/*Body* pBody = pScript->mpOwner->GetComponent<Body>();
		if (mScripts.find("Move.lua") == mScripts.end()) {
			sol::load_result script = lua.load_file(directory::script + "Move.lua");
				
			if (!script.valid()) {
				sol::error err = script;
				std::string what = err.what();
				LOG_INFO("FlyEngine", lua_file + ". Script loading invalid: " + what);
			}
			else {
				mScripts["Move.lua"] = script;
					
				sol::protected_function_result scriptResult = script();
				if (!scriptResult.valid()) {
					sol::error err = scriptResult;
					std::string what = err.what();
					LOG_INFO("FlyEngine", lua_file + ". Script result invalid: " + what);
				}
			}
		}
		else {
			if (mScripts["Move.lua"].valid()) {
				int x = 0;
			}
			sol::protected_function_result scriptResult = mScripts["Move.lua"]();
			if (!scriptResult.valid()) {
				sol::error err = scriptResult;
				std::string what = err.what();
				LOG_INFO("FlyEngine", lua_file + ". Script result invalid: " + what);
			}
		}*/
			





		// Lua fn from C++ : BehaviorSentinel.lua file   ------------------------------------------------------
		if ("BehaviorSentinel.lua" == lua_file) {
			
			sol::table tab = lua[std::to_string(c->mpOwner->id)];

			Body* pBody = pScript->mpOwner->GetComponent<Body>();
			if (pBody) {

				float y = tab["y"];
				// do something with y

				lua["x"] = tab["x"];
				float x = sentinelVelocity(deltaTime);
				pBody->mPos.x += x;
				if(pBody->pTr)
					pBody->pTr->mDirtyFlag = true;
				tab["x"] = lua["x"];

			}

		}

		c = c->GetNext();
	}
}