/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "../InitData.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "ObjectFactory.h"
#include "../Utilities/Reflection.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "../Systems/RenderCalculation.h"
#include "../Systems/PlayerSystem.h"
#include "../Systems/ControlSystem.h"
extern ResourceManager* gpResourceManager;
extern ObjectFactory* gpObjectFactory;
extern Reflection* gpReflection;
extern EntityManager* gpEntityManager;
extern AudioManager* gpAudioManager;
extern SystemManager* gpSystemManager;
extern CameraManager* gpCameraManager;
extern bool isPaused;
extern bool gameStarted;
SceneManager::SceneManager()
{
	next_scene = FlyEngine::General::start_scene;
}


SceneManager::~SceneManager()
{
	for (auto doc : mLevelPrefabs) {
		delete doc.second;
	}
	mLevelPrefabs.clear();
}

void SceneManager::LoadNextLevel()
{
	gpAudioManager->StopAll();

	auto begin = FlyEngine::General::mLevelSequence.begin();
	auto end = FlyEngine::General::mLevelSequence.end();

	auto itr = std::find(begin, end, current_scene);
	if (itr != end) {
		if ((itr + 1) != end) {
			setNextScene(*(itr+1));
		}
	}

	if (next_scene == "WinScreen.json") {
		gameStarted = false;
		gpCameraManager->SnapToPosition(glm::vec3( 40.0f,9.0f,150.0f) );
		gpSystemManager->mControlSystem->selectedItem = Playagain;
		gpSystemManager->mControlSystem->menuStartingIndex = Playagain;
		gpSystemManager->mControlSystem->menuLength = 2;
	}
}

void SceneManager::LoadLevel(std::string _filename)
{
	for (auto e : mMenuEntity) {
		//Fullscreen Text (on/off)
		if (e->mType == FULLSCREEN_ONOFF) {
			gpSystemManager->mControlSystem->isSettingPage = true;
			gpSystemManager->mControlSystem->Fullscreen_Text = nullptr;
		}

		//Mute Text (on/off)
		if (e->mType == MUTE_ONOFF) {
			gpSystemManager->mControlSystem->isSettingPage = true;
			gpSystemManager->mControlSystem->Mute_Text = nullptr;
		}
		gpObjectFactory->DeleteObject(e->id);
	}
	mMenuEntity.clear();
	
	Document* pDoc = nullptr;
	std::string fullPath = directory::level + _filename;
	pDoc = gpResourceManager->ReadJSON(fullPath, pDoc);
	if (pDoc) {
		mLevelPrefabs[_filename] = pDoc;
		if (_filename == "PauseMenu.json" || 
			_filename == "ControlPauseMenu.json" ||
			_filename == "Menu.json" ||
			_filename == "LoadLevelScreen.json" ||
			_filename == "CreditMenu.json" ||
			_filename == "GameExit_Confirmation.json" ||
			_filename == "MainMenu_Confirmation.json" ||
			_filename == "SettingsMenu.json" ||
			_filename == "SettingsMenuPause.json" ||
			_filename == "WinScreen.json" ||
			_filename == "ControlMenu.json")
			gpReflection->Deserialize(pDoc, true);
		else {
			gpReflection->Deserialize(pDoc);
		}
		gpSystemManager->mRenderCalculation->CalculateModelingMatrix();
	}

	if (_filename == "SettingsMenuPause.json" || _filename == "SettingsMenu.json") {
		gpSystemManager->mControlSystem->isSettingPage = true;
		for (auto e : mMenuEntity) {
			//Fullscreen Text (on/off)
			if (e->mType == FULLSCREEN_ONOFF) {
				gpSystemManager->mControlSystem->Fullscreen_Text = e;
			}
			//Mute Text (on/off)
			if (e->mType == MUTE_ONOFF) {
				gpSystemManager->mControlSystem->Mute_Text = e;
			}
		}
	}


	//Shashwat check this out
	if (_filename == "LevelBoss.json") {
		gpCameraManager->keepFollowing = true;
	}
}

 std::string SceneManager::getLastSerializedScene(unsigned int i)  {
	 return gpReflection->GetNameFromSlot(i);
 }

void SceneManager::ClearLevel()
{
	for (auto itr = gpEntityManager->mEntity.begin(); itr != gpEntityManager->mEntity.end(); ++itr){
		gpObjectFactory->DeleteObject(itr->second->id);
	}
	gpAudioManager->StopAll();
	gpAudioManager->mLoadLoopChannels.clear();
	gpSystemManager->mPlayerSystem->mCpVec.clear();
	gpSystemManager->mPlayerSystem->SetPlayerEntity(nullptr);
}

void SceneManager::Update()
{
	if (LoopPassed) {
		LoadLevel(next_scene);
		LoopPassed = false;
	}
	//if   the name of the scene changed or  whant to reload the same level
	if (current_scene != next_scene || sameLevel) {
		ClearLevel();
		current_scene = next_scene;
		LoopPassed = true;
		sameLevel = false;
	}
}