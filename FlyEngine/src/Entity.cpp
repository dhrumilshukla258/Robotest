/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Author: Dhrumil Shukla, dhrumil.shukla 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#include "pch.h"

#include "Entity.h"

RTTR_REGISTRATION
{
	rttr::registration::enumeration<EntityType>("EntityType")
		 (
			 rttr::value("Player", EntityType::PLAYER),
			 rttr::value("Boss", EntityType::BOSS),

			 //Types of Gun
			 rttr::value("FireGun", EntityType::FIRE_GUN),
			 rttr::value("IceGun", EntityType::ICE_GUN),

			 //Types of Bullet
			 rttr::value("FireBullet", EntityType::FIRE_BULLET),

			 //Enemy Types and its Resources
			 rttr::value("Enemy", EntityType::ENEMY),
			 rttr::value("EnemyTrigger", EntityType::ENEMY_TRIGGER),
			 rttr::value("EnemyWayPoint", EntityType::ENEMY_WAY_POINT),
			 rttr::value("EnemyBullet", EntityType::ENEMY_BULLET),

			 //Types of Buttons
			 rttr::value("MouseClick", EntityType::MOUSE_CLICK),
			 rttr::value("Level1", EntityType::LEVEL_1),
			 rttr::value("Level2", EntityType::LEVEL_2),
			 rttr::value("Level3", EntityType::LEVEL_3),
			 rttr::value("LevelBoss", EntityType::LEVEL_BOSS),
			 rttr::value("NewGame", EntityType::NEW_GAME),
			 rttr::value("ControlMenu", EntityType::CONTROL_MENU),
			 rttr::value("ControlPauseMenu", EntityType::CONTROL_PAUSE_MENU),
			 rttr::value("SettingsMenu", EntityType::SETTINGS_MENU),
			 rttr::value("SettingsMenuPause", EntityType::SETTINGS_MENU_PAUSE),
			 rttr::value("CreditMenu", EntityType::CREDIT_MENU),
			 rttr::value("LoadLevel", EntityType::LOAD_LEVEL),
			 rttr::value("Resume", EntityType::RESUME),
			 rttr::value("BackMainMenu", EntityType::BACK_MAIN_MENU),
			 rttr::value("BackPauseMenu", EntityType::BACK_PAUSE_MENU),
			 rttr::value("Button_Exit", EntityType::EXIT),

			 rttr::value("FullscreenOnOff", EntityType::FULLSCREEN_ONOFF),
			 rttr::value("MuteOnOff", EntityType::MUTE_ONOFF),

			 rttr::value("MuteButton", EntityType::MUTE_BUTTON),
			 rttr::value("FullscreenButton", EntityType::FULLSCREEN_BUTTON),
			 
			 rttr::value("ActualExit", EntityType::ACTUAL_EXIT),

			 //Types of Boxes
			 rttr::value("Box", EntityType::BOX),

			 //Types of Key
			 rttr::value("Key", EntityType::KEY),

			 //Type of Doors
			 rttr::value("Door", EntityType::DOOR),
			 rttr::value("BurnDoor", EntityType::BURN_DOOR),

			 //Different Game Objects
			 rttr::value("World", EntityType::WORLD),
			 rttr::value("Spikes", EntityType::SPIKES),
			 rttr::value("Launcher", EntityType::LAUNCHER),
			 rttr::value("Drone", EntityType::DRONE),
			 rttr::value("Rope", EntityType::ROPE),
			 rttr::value("TriggerBox", EntityType::TRIGGER_BOX),

			 //Stable Platform
			 rttr::value("Platform_Blue", EntityType::PLATFORM_BLUE),
			 rttr::value("Platform_DarkBlue", EntityType::PLATFORM_DARKBLUE),
			 rttr::value("Platform_Gray", EntityType::PLATFORM_GRAY),

			 //Moving Platform
			 rttr::value("MovingPlatform", EntityType::MOVINGPLATFORM),
			 rttr::value("MovingSpikes", EntityType::MOVINGSPIKES),

			 //Light Objects
			 rttr::value("Light", EntityType::LIGHT),
			 rttr::value("StreetLamp", EntityType::STREET_LAMP),
			 rttr::value("Spot_Light", EntityType::SPOT_LIGHT_1),
			 rttr::value("Blast_Effect", EntityType::BLAST_EFFECT),
			 rttr::value("Flame_Throw", EntityType::FLAME_THROW),
			 rttr::value("Checkpoint", EntityType::CHECKPOINT),

			 //Level End Object
			 rttr::value("End", EntityType::END),

			 //Background Objects
			 rttr::value("Background_1", EntityType::BACKGROUND_1),
			 rttr::value("Skybox", EntityType::SKY_BOX),
			 rttr::value("BG_Table", EntityType::BG_TABLE),
			 rttr::value("BG_Chair", EntityType::BG_CHAIR),
			 rttr::value("BG_Box_Blue", EntityType::BG_BOX_BLUE),

			 //////////Foreground
			 rttr::value("Foreground", EntityType::FOREGROUND),
			 //////////Background
			 rttr::value("Background", EntityType::BACKGROUND),
			 //////////Particle
			 rttr::value("Particle", EntityType::PARTICLE),

			 rttr::value("TextBox", EntityType::TEXT_BOX),
			 rttr::value("TextTrigger", EntityType::TEXT_TRIGGER),
			 rttr::value("DroneTrigger", EntityType::DRONE_TRIGEGR),
			 rttr::value("CameraTrigger", EntityType::CAMERA_TRIGGER),
			 rttr::value("DPLogo", EntityType::DP_LOGO),
			 rttr::value("credit_1", EntityType::CREDIT_1),
			 rttr::value("credit_2", EntityType::CREDIT_2),
			 rttr::value("credit_3", EntityType::CREDIT_3),
			 rttr::value("credit_4", EntityType::CREDIT_4),
			  rttr::value("control", EntityType::CONTROL),
			 rttr::value("fmod", EntityType::FMOD_LOGO),
			 rttr::value("fireflieslogotext", EntityType::FIREFLIES_LOGO_TEXT),
			 rttr::value("fireflieslogo", EntityType::FIREFLIES_LOGO_NO_TEXT),
			 rttr::value("robotestlogo", EntityType::ROBOTEST_LOGO),
			 rttr::value("ControllerControls", EntityType::CONTROLLER_CONTROLS),
			 rttr::value("ControllerExperience", EntityType::CONTROLLER_EXPERIENCE)
			 

		);
}

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::AddComponent(Component * _pCompo)
{
	mComponents.push_back(_pCompo);
}

void Entity::ResetComponent()
{
	for (unsigned int i = 0; i < mComponents.size(); ++i) {
		mComponents[i]->Init();
	}
}