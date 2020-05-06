/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Author: Dhrumil Shukla, dhrumil.shukla 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#pragma once
#include "Component.h"

enum EntityType {
	PLAYER,
	BOSS,
	PLATFORM,
	
	FIRE_BULLET,
	ICE_BULLET,
	ENEMY_BULLET,
	SKY_BOX,
	BOX,
	ENEMY,
	ROPE,
	NCPLATFORM,
	MOVINGPLATFORM,
	MOVINGSPIKES,
	BACKGROUND_1,
	TRIGGER_BOX,
	BG_BOX_BLUE,

	PLATFORM_BLUE,
	PLATFORM_DARKBLUE,
	PLATFORM_GRAY,
	
	MOUSE_CLICK,
	NEW_GAME,
	LOAD_LEVEL,
	CONTROL_MENU,
	CONTROL_PAUSE_MENU,
	SETTINGS_MENU,
	SETTINGS_MENU_PAUSE,
	CREDIT_MENU,
	RESUME,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	LEVEL_BOSS,
	BACK_MAIN_MENU,
	BACK_PAUSE_MENU,
	FULLSCREEN_BUTTON,
	MUTE_BUTTON,
	EXIT,
	ACTUAL_EXIT,

	//Sorry But neet to use this #DirtyCode Again
	FULLSCREEN_ONOFF,
	MUTE_ONOFF,


	DOOR,
	BURN_DOOR,
	
	KEY,
	SPIKES,
	LAUNCHER,
	LIGHT,
	WORLD,
	ENEMY_TRIGGER,
	ENEMY_WAY_POINT,
	BG_TABLE,
	BG_CHAIR,
	STREET_LAMP,
	SPOT_LIGHT_1,
	FIRE_GUN,
	ICE_GUN,
	DRONE,
	BLAST_EFFECT,
	FLAME_THROW,
	CHECKPOINT,
	BACKGROUND,
	FOREGROUND,
	PARTICLE,
	END,

	TEXT_BOX,
	TEXT_TRIGGER,
	DRONE_TRIGEGR,
	CAMERA_TRIGGER,
	DOOR_CLOSE_TRIGGER,
	DP_LOGO,
	CREDIT_1,
	CREDIT_2,
	CREDIT_3,
	CREDIT_4,
	CONTROL,
	FMOD_LOGO,
	FIREFLIES_LOGO_TEXT,
	FIREFLIES_LOGO_NO_TEXT,
	ROBOTEST_LOGO,
	CONTROLLER_CONTROLS,
	CONTROLLER_EXPERIENCE,



};

class Entity
{
public:
	Entity();
	~Entity();
	void AddComponent(Component*);
	void ResetComponent();
	template <typename T>
	T* GetComponent() {
		T x;
		for (auto& mC : mComponents) {
			if (mC->mComponentName == x.mComponentName)
				return static_cast<T*>(mC);
		}
		return nullptr;
	}
public:
	unsigned int id;
	Entity* mParent = nullptr;
	Entity* mChild = nullptr;
	EntityType mType;
	std::vector<Component*> mComponents;

	Entity* mOwnerEntity = nullptr;
	std::vector<Entity*> mChildEntities;

protected:
	RTTR_ENABLE()
};