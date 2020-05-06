/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "ControlSystem.h"
#include "../Managers/SceneManager.h"
#include "../Managers/InputManager.h"
#include "../Managers/ObjectFactory.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/FrameRateController.h"
#include "../Managers/AudioManager.h"
#include "../Managers/CameraManager.h"
#include "../Utilities/RayCast.h"

#include "../Components/Model.h"
#include "../Entity.h"

extern FrameRateController* gpFrameRateController;
extern InputManager* gpInputManager;
extern ComponentManager* gpComponentManager;
extern SceneManager* gpSceneManager;
extern ObjectFactory* gpObjectFactory;
extern AudioManager* gpAudioManager;
extern CameraManager* gpCameraManager;
extern RayCast* gpRayCast;
extern GLFWwindow* gpWindow;
extern GLFWmonitor* monitor;
extern float SCR_HEIGHT;
extern bool gameStarted;
extern bool isPaused;
extern bool flipScreenMode;
extern float SCR_WIDTH;
extern float SCR_HEIGHT;
float totalTime = 0.0f;
int index = 0;

// Hard Coded Camera Movement Values Ahead : Tread Carefully
const int firstLoadScreensCount = 5;
const float firstLoadLoopTime = 2.5f;
extern bool isAllMute;

const glm::vec3 firstLoadPositions[] = {
	glm::vec3(250.0f, 5.0f, 150.0f),
	glm::vec3(250.0f, 59.0f, 150.0f),
	glm::vec3(250.0f, 135.0f, 150.0f),		
	glm::vec3(250.0f, 200.0f, 150.0f),		
	glm::vec3(40.0f, 9.0f, 150.0f)		
};

const int creditScreensCount = 5;
const float creditsLoopTime = 5.0f;
const glm::vec3 creditsPositions[] = {
	glm::vec3(-250.0f, 9.0f, 150.0f),
	glm::vec3(-250.0f, 59.0f, 150.0f),
	glm::vec3(-250.0f, 109.0f, 150.0f),
	glm::vec3(-250.0f, 159.0f, 150.0f),
	glm::vec3(40.0f, 9.0f, 150.0f)
};
// Hard Code Ends

ControlSystem::ControlSystem() :menuLength(0), selectedItem(0), menuStartingIndex(0), counter(0)
{
	indexToLevelMap[NewGame] = std::make_pair<int, string>(0, "Level01.json");
	indexToLevelMap[Level1] = std::make_pair<int, string>(0, "Level01.json");
	indexToLevelMap[Level2] = std::make_pair<int, string>(0, "Level02.json");
	indexToLevelMap[Level3] = std::make_pair<int, string>(0, "Level03.json");
	indexToLevelMap[LevelBoss] = std::make_pair<int, string>(0, "LevelBoss.json");
	indexToLevelMap[LoadLevel] = std::make_pair<int, string>(5, "LoadLevelScreen.json");
	indexToLevelMap[Controls] = std::make_pair<int, string>(1, "ControlMenu.json");
	indexToLevelMap[Credits] = std::make_pair<int, string>(1, "CreditMenu.json");
	
	indexToLevelMap[Resume] = std::make_pair<int, string>(0, "");
	indexToLevelMap[ControlsP] = std::make_pair<int, string>(1, "ControlPauseMenu.json");
	
	indexToLevelMap[BackP] = std::make_pair<int, string>(5, "PauseMenu.json");
	indexToLevelMap[Back_To_Pause_From_Setting] = std::make_pair<int, string>(5, "PauseMenu.json");

	indexToLevelMap[MainMenu] = std::make_pair<int, string>(6, "Menu.json");
	indexToLevelMap[Back] = std::make_pair<int, string>(6, "Menu.json");
	indexToLevelMap[Back_To_Menu_From_Setting] = std::make_pair<int, string>(6, "Menu.json");

	indexToLevelMap[Playagain] = std::make_pair<int, string>(0, "Level01.json");
	indexToLevelMap[Win_To_Mainmenu] = std::make_pair<int, string>(6, "Menu.json");


	indexToLevelMap[Exit] = std::make_pair<int, string>(2, "GameExit_Confirmation.json");
	indexToLevelMap[Settings] = std::make_pair<int, string>(3, "SettingsMenu.json");
	//indexToLevelMap[SettingsP] = std::make_pair<int, string>(3, "SettingsMenuPause.json");

	indexToLevelMap[MainMenu_Exit_No] = std::make_pair<int, string>(6, "Menu.json");
	indexToLevelMap[PauseMenu_Exit_Yes] = std::make_pair<int, string>(6, "Menu.json");
	indexToLevelMap[PauseMenu_Exit_No] = std::make_pair<int, string>(5, "PauseMenu.json");

	// initialize loading
	gpCameraManager->SetTweenToPosition(firstLoadPositions[0]);

	screenPosArray = firstLoadPositions;
	screenPosCount = firstLoadScreensCount;
	screenLoopTime = firstLoadLoopTime;
	totalTime = screenLoopTime;
	loopScreens = true;
	selectedItem = -1;

	
}

void ControlSystem::ReadyMainMenu() {
	if (!gameStarted) {
		// Main menu setup
		menuLength = indexToLevelMap[Back].first;
		menuStartingIndex = NewGame;
		selectedItem = menuStartingIndex;
	}
	else {
		menuLength = indexToLevelMap[BackP].first;
		menuStartingIndex = Resume;
		selectedItem = menuStartingIndex;
	}

}

void ControlSystem::UpdateControlScreen() {
	Entity* controllerScreen = nullptr , *keyboardScreen = nullptr;
	for (auto& en : gpSceneManager->mMenuEntity) {
		if (en->mType == CONTROLLER_CONTROLS)
			controllerScreen = en;

		if (en->mType == CONTROL)
			keyboardScreen = en;
	}

	if (controllerScreen && keyboardScreen) {
		if (gpInputManager->isJoyStick) {
			controllerScreen->GetComponent<Transform>()->mPos.z = 91.0f;
			keyboardScreen->GetComponent<Transform>()->mPos.z = 90.0f;
		}
		else {
			controllerScreen->GetComponent<Transform>()->mPos.z = 90.0f;
			keyboardScreen->GetComponent<Transform>()->mPos.z = 91.0f;
		}
	}
}

ControlSystem::~ControlSystem()
{
}

const glm::vec3 GLOW_COLOR = glm::vec3(4.2f, 0.1f, 0.0f);

void ControlSystem::GlowSelectedItem(int index) {
	for (unsigned int i = 0; i < gpSceneManager->mMenuEntity.size(); ++i) {
		Model* pModel = gpSceneManager->mMenuEntity[i]->GetComponent<Model>();
		if (pModel != nullptr) {
			if (gpSceneManager->mMenuEntity[i]->mType == NEW_GAME && ( index == NewGame || index == Playagain )||
				gpSceneManager->mMenuEntity[i]->mType == LOAD_LEVEL && index == LoadLevel || 
				gpSceneManager->mMenuEntity[i]->mType == CREDIT_MENU && (index == Credits || index == CreditsP) || 
				gpSceneManager->mMenuEntity[i]->mType == SETTINGS_MENU && index == Settings || 
				gpSceneManager->mMenuEntity[i]->mType == SETTINGS_MENU_PAUSE && index == SettingsP ||
				gpSceneManager->mMenuEntity[i]->mType == CONTROL_MENU && index == Controls || 
				gpSceneManager->mMenuEntity[i]->mType == EXIT && index == Exit || 
				gpSceneManager->mMenuEntity[i]->mType == LEVEL_1 && index == Level1 ||
				gpSceneManager->mMenuEntity[i]->mType == LEVEL_2 && index == Level2 ||
				gpSceneManager->mMenuEntity[i]->mType == LEVEL_3 && index == Level3 ||
				gpSceneManager->mMenuEntity[i]->mType == LEVEL_BOSS && index == LevelBoss ||
				gpSceneManager->mMenuEntity[i]->mType == CONTROL_PAUSE_MENU && index == ControlsP ||
				gpSceneManager->mMenuEntity[i]->mType == BACK_PAUSE_MENU && ( index == BackP || index == PauseMenu_Exit_No || index == Back_To_Pause_From_Setting) ||
				gpSceneManager->mMenuEntity[i]->mType == BACK_MAIN_MENU &&  ( index == Win_To_Mainmenu || index == Back || index == MainMenu_Exit_No || index == MainMenu || index == PauseMenu_Exit_Yes || index == Back_To_Menu_From_Setting) ||
				gpSceneManager->mMenuEntity[i]->mType == RESUME && index == Resume ||
				gpSceneManager->mMenuEntity[i]->mType == ACTUAL_EXIT && index == MainMenu_Exit_Yes ||
				
				gpSceneManager->mMenuEntity[i]->mType == MUTE_BUTTON && ( index == Mute || index == MuteP ) ||
				gpSceneManager->mMenuEntity[i]->mType == FULLSCREEN_BUTTON && ( index == Fullscreen || index == FullScreenP )


				) {
				pModel->mCol = GLOW_COLOR;
			}
			else {
				pModel->mCol =pModel->originalColor;
			}
		}
	}
}

void ControlSystem::Update()
{
	std::string Current_Scene=  gpSceneManager->getCurrentScene();
	if (isSettingPage) {
		if (Mute_Text != nullptr) {
			Text* p_m_Tex = Mute_Text->GetComponent<Text>();
			if (isMuteYes) {
				p_m_Tex->mStr = "On";
			}
			else {
				p_m_Tex->mStr = "Off";
			}
		}

		//If FlipScreenMode is false then fullscreen
		if (Fullscreen_Text != nullptr) {
			Text* p_f_Tex = Fullscreen_Text->GetComponent<Text>();
			if (flipScreenMode)
				p_f_Tex->mStr = "Off";
			else
				p_f_Tex->mStr = "On";
		}
	}

	if (loopScreens) {
		if (index == screenPosCount) {
			loopScreens = false;
			index = 0;
			ReadyMainMenu();
			return;
		}
		totalTime += (float)gpFrameRateController->GetFrameTime();
		if (totalTime > screenLoopTime) {
			totalTime = 0.0f;
			gpCameraManager->SetTweenToPosition(*(screenPosArray + index++));
		}
		if (gpInputManager->isTriggered(ENTER) || 
			gpInputManager->isTriggered(JUMP) ||
			gpInputManager->isTriggered(ESC)) {
			// if pre menu sequence
			if (selectedItem == -1) {
				// skip
				loopScreens = false;
				index = 0;
				gpCameraManager->SetTweenToPosition(firstLoadPositions[firstLoadScreensCount-1]);
				ReadyMainMenu();
				return;
			}
		}
		return;
	}
		
	if (!gameStarted || isPaused) {
		if (gpSceneManager->getCurrentScene() == "ControlMenu.json" ||
			menuLevelName == "ControlPauseMenu.json") {
			UpdateControlScreen();
		}

		if (gpInputManager->isTriggered(DOWN)) {
			++counter;
			counter = counter > (menuLength - 1) ? 0 : counter;
			selectedItem = menuStartingIndex + counter;
		}
		else if (gpInputManager->isTriggered(UP)) {
			--counter;
			counter = counter < 0 ? menuLength - 1 : counter;
			selectedItem = menuStartingIndex + counter;
		}
		else if (gpInputManager->isTriggered(ENTER) || gpInputManager->isTriggered(JUMP)) {
			counter = 0;

			if (selectedItem == Resume) {
				isPaused = false;
				
				if (isMuteYes == false) {
					if (isAllMute == true)
						gpAudioManager->ToggleMuteAll();
				}
				else {
					if (isAllMute == false)
						gpAudioManager->ToggleMuteAll();
				}
				
				for (auto e : gpSceneManager->mMenuEntity) {
					gpObjectFactory->DeleteObject(e->id);
				}
				gpSceneManager->mMenuEntity.clear();

				return;
			}
			else if(selectedItem == Credits || selectedItem == CreditsP) {
				loopScreens = true;
				screenPosArray = creditsPositions;
				screenPosCount = creditScreensCount;
				screenLoopTime = creditsLoopTime;
				totalTime = screenLoopTime;
				selectedItem = -1;
				return;
			}
			else if (selectedItem == ControlsP) {
				gpSceneManager->LoadLevel("ControlPauseMenu.json");
				menuLevelName = "ControlPauseMenu.json";

				menuLength = 1;
				selectedItem = BackP;
				menuStartingIndex = BackP;
				return;
			}
			else if (selectedItem == BackP || selectedItem == PauseMenu_Exit_No || selectedItem == Back_To_Pause_From_Setting) {
				gpSceneManager->LoadLevel("PauseMenu.json");
				menuLevelName = "PauseMenu.json";
				menuLength = indexToLevelMap[BackP].first;
				menuStartingIndex = Resume;
				selectedItem = menuStartingIndex;
				return;
			}
			else if (selectedItem == MainMenu) {
				gpSceneManager->LoadLevel("MainMenu_Confirmation.json");

				menuLength = indexToLevelMap[Exit].first;
				selectedItem = PauseMenu_Exit_No;
				menuStartingIndex = PauseMenu_Exit_No;
				return;
			}
			else if (selectedItem == PauseMenu_Exit_Yes || selectedItem == Back_To_Menu_From_Setting) {
				selectedItem = MainMenu;
				gameStarted = false;
			}
			else if (selectedItem == SettingsP) {
				gpSceneManager->LoadLevel("SettingsMenuPause.json");

				menuLength = 3;
				selectedItem = MuteP;
				menuStartingIndex = MuteP;
				return;
			}
			else if (selectedItem == Mute || selectedItem == MuteP) {
				if (isMuteYes == true) {
					isMuteYes = false;
				}
				else {
					isMuteYes = true;
				}
				return;
			}
			else if (selectedItem == Fullscreen || selectedItem == FullScreenP) {
  				if (flipScreenMode) {
					glfwSetWindowMonitor(gpWindow, monitor, 0, 0, (int)SCR_WIDTH, (int)SCR_HEIGHT, 60);
				}
				else {
					glfwSetWindowMonitor(gpWindow, NULL, 5, 40, (int)SCR_WIDTH, (int)SCR_HEIGHT, 60);
				}
				flipScreenMode = !flipScreenMode;

				selectedItem = Mute;
				return;
			}
			menuLength = indexToLevelMap[selectedItem].first;

			gpSceneManager->setNextScene(indexToLevelMap[selectedItem].second.c_str());

			if (menuLength == 1) { // implies back key
				if (selectedItem < Resume) {
					selectedItem = Back;
					menuStartingIndex = Back;
					return;
				}
			}

			if (selectedItem == NewGame || selectedItem >= Level1 && selectedItem <= LevelBoss || selectedItem == Playagain) {
				gameStarted = true;
				isPaused = false;

				if (isMuteYes == false) {
					if (isAllMute == true)
						gpAudioManager->ToggleMuteAll();
				}
				else {
					if (isAllMute == false)
						gpAudioManager->ToggleMuteAll();
				}

				// get ready for pause menu
				menuLength = indexToLevelMap[BackP].first;
				selectedItem = Resume;
				menuStartingIndex = Resume;
			}

			switch (selectedItem) {
			case MainMenu: {
				menuStartingIndex = NewGame;
				selectedItem = menuStartingIndex;
				break;
			}
			case LoadLevel: {
				menuStartingIndex = Level1;
				selectedItem = menuStartingIndex;
				break;
			}
			case Win_To_Mainmenu:
			case MainMenu_Exit_No:
			case Back: {
				menuStartingIndex = NewGame;
				selectedItem = menuStartingIndex;
				break;
			}
			case Settings: {
				menuStartingIndex = Mute;
				selectedItem = menuStartingIndex;
				break;
			}
			case Exit: {
				menuStartingIndex = MainMenu_Exit_No;
				selectedItem = menuStartingIndex;
				break;
			}
			case MainMenu_Exit_Yes: {
				glfwSetWindowShouldClose(gpWindow, true);
				break;
			}
			
			}
		}

		GlowSelectedItem(selectedItem);
	}

#ifdef _DEBUG
	//bool open = true;
	//ImGui::SetNextWindowPos(ImVec2(1096, 0));
	ImGui::Begin("Frames!");// , &open, ImVec2(344, 105), 0.9f, ImGuiWindowFlags_NoResize); // Create a window called "Hello, world!" and append into it.
	//ImGui::SetWindowSize(ImVec2(344, 105));

	ImGui::Text("SelectedItem = %d", selectedItem);
	

	ImGui::End();
#endif
	/*return;

	Component* c = gpComponentManager->mControl->GetHead();
	while (nullptr != c) {
		Control* pCr = static_cast<Control*>(c);
		Body* pBody = pCr->mpOwner->GetComponent<Body>();
		if (nullptr != pBody) {
			if (gpInputManager->isTriggered(MOUSE_LEFT)) {
				Entity * e = gpRayCast->CastThisShit();
				if (e) {
					if (e->mType == LOAD_LEVEL) {
						gpSceneManager->setNextScene("LoadLevelScreen.json");
						break;
					}

					if (e->mType == CREDIT_MENU) {
						gpSceneManager->setNextScene("CreditMenu.json");
						break;
					}

					if (e->mType == CONTROL_MENU) {
						gpSceneManager->setNextScene("ControlMenu.json");
						break;
					}

					if (e->mType == NEW_GAME || e->mType == LEVEL_1) {
						gameStarted = true;
						isPaused = false;
						gpSceneManager->setNextScene("Level01.json");
						break;
					}
					if (e->mType == LEVEL_2) {
						gameStarted = true;
						isPaused = false;
						gpSceneManager->setNextScene("Level02.json");
						break;
					}
					if (e->mType == LEVEL_3) {
						gameStarted = true;
						isPaused = false;
						gpSceneManager->setNextScene("Level03.json");
						break;
					}
					if (e->mType == LEVEL_BOSS) {
						gameStarted = true;
						isPaused = false;
						gpSceneManager->setNextScene("LevelBoss.json");
						break;
					}

					if (e->mType == RESUME) {
						isPaused = false;
						gpAudioManager->ToggleAll();

						for (auto e : gpSceneManager->mMenuEntity) {
							gpObjectFactory->DeleteObject(e->id);
						}
						gpSceneManager->mMenuEntity.clear();
						break;
					}
					
					if (e->mType == BACK_MAIN_MENU) {
						gameStarted = false;
						gpAudioManager->StopAll();

						for (auto e : gpSceneManager->mMenuEntity) {
							gpObjectFactory->DeleteObject(e->id);
						}
						gpSceneManager->mMenuEntity.clear();

						
						gpSceneManager->setNextScene("Menu.json");
						break;
					}

					if (e->mType == CONTROL_PAUSE_MENU) {
						for (auto e : gpSceneManager->mMenuEntity) {
							gpObjectFactory->DeleteObject(e->id);
						}
						gpSceneManager->mMenuEntity.clear();

						gpSceneManager->LoadLevel("ControlPauseMenu.json");
						break;
					}
					if (e->mType == BACK_PAUSE_MENU) {
						for (auto e : gpSceneManager->mMenuEntity) {
							gpObjectFactory->DeleteObject(e->id);
						}
						gpSceneManager->mMenuEntity.clear();

						gpSceneManager->LoadLevel("PauseMenu.json");
						break;
					}

					if (e->mType == EXIT)
						glfwSetWindowShouldClose(gpWindow, true);
				}
			}
		}
		c = c->GetNext();
	}*/
 }