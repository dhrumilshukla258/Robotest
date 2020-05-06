/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#pragma once

#include "../Component.h"
#include "../Components/Transform.h"
#include "../Components/Sprite.h"
#include "../Components/Body.h"
#include "../Components/Model.h"
#include "../Components/RawModel.h"
#include "../Components/BackgroundModel.h"
#include "../Components/Camera.h"
#include "../Components/Control.h"
#include "../Components/Collider.h"
#include "../Components/Health.h"
#include "../Components/Timer.h"
#include "../Components/Particle.h"
#include "../Components/ParticleGenerator.h"
#include "../Components/Oscillation.h"
#include "../Components/Key.h"
#include "../Components/Script.h"
#include "../Components/Launcher.h"
#include "../Components/Door.h"
#include "../Components/Homing.h"
#include "../Components/Light.h"
#include "../Components/World.h"
#include "../Components/Stencil_Toon_Model.h"
#include "../Components/EnemyAI.h"
#include "../Components/Animation.h"
#include "../Components/Text.h"
#include "../Components/Boss.h"
#include "../Utilities/MapListStructure.h"

class ShapeStructure;
class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();
	void DeleteComponent(MapListStructure*, unsigned int);
	void Init(MapListStructure*);
	MapListStructure* ComponentConstructor(MapListStructure*);
	void ComponentDestructor(MapListStructure*);

	void invoke(std::string, unsigned int id=0);
public:
	//Component Pointers
	MapListStructure* mTransform;
	MapListStructure* mSprite;
	MapListStructure* mBody;
	MapListStructure* mRawModel;
	MapListStructure* mBackground_Model;
	MapListStructure* mModel;
	MapListStructure* mControl;
	MapListStructure* mCamera;
	MapListStructure* mHealth;
	MapListStructure* mTimer;
	MapListStructure* mParticle;
	MapListStructure* mParticleGenerator;
	MapListStructure* mOscillation;
	MapListStructure* mKey;
	MapListStructure* mCollider;
	MapListStructure* mScript;
	MapListStructure* mLauncher;
	MapListStructure* mDoor;
	MapListStructure* mHoming;
	MapListStructure* mLight;
	MapListStructure* mWorld;
	MapListStructure* mStencil_Toon_Model;
	MapListStructure* mEnemyAI;
	MapListStructure* mAnimation;
	MapListStructure* mText;
	MapListStructure* mBoss;
	
	//Shape Pointers
	ShapeStructure* mShapeAABB;
	ShapeStructure* mShapeCircle;
protected:
	RTTR_ENABLE()
};