#pragma once

enum MainMenu {
	NewGame = 0,
	LoadLevel,
	Controls,
	Settings,
	Credits,
	Exit,

	Level1 = 6,
	Level2,
	Level3,
	LevelBoss,
	Back,

	Resume = 11,
	ControlsP,
	SettingsP,
	CreditsP,
	MainMenu,
	
	MainMenu_Exit_No = 16,
	MainMenu_Exit_Yes,

	PauseMenu_Exit_No = 18,
	PauseMenu_Exit_Yes,

	Mute = 20,
	Fullscreen,
	Back_To_Menu_From_Setting,

	MuteP = 23,
	FullScreenP,
	Back_To_Pause_From_Setting,

	Playagain = 26,
	Win_To_Mainmenu,

	BackP
};

class Entity;
class ControlSystem
{
public:
	int selectedItem;
	int menuStartingIndex;
	int menuLength;
	
	std::string menuLevelName;
	int counter;

	std::unordered_map<int, std::pair<int, std::string>> indexToLevelMap;

	bool loopScreens;
	const glm::vec3* screenPosArray;
	int screenPosCount;
	float screenLoopTime;

	void GlowSelectedItem(int);
	void ReadyMainMenu();
	void UpdateControlScreen();
	ControlSystem();
	~ControlSystem();
	void Update();

	Entity * Mute_Text = nullptr;
	Entity * Fullscreen_Text = nullptr;
	bool isSettingPage = false;
	bool isMuteYes = false;
};

