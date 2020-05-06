/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Launcher.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::enumeration<MissileType>("MissileType")
	(
		rttr::value("ShootBullet", MissileType::ShootBullet),
		rttr::value("ShootHoming", MissileType::ShootHoming),
		rttr::value("Drone", MissileType::Drone)
	);

	rttr::registration::class_<Launcher>("Launcher")
		 .constructor()
		 .property("Time", &Launcher::mTimer)
		 .property("MissileType", &Launcher::mMissileType)
		 .property("MissileSpeed", &Launcher::mMissileSpeed);
}

Launcher::Launcher() : mTimer(0.0f), tempTime(0.0f), mMissileSpeed(0.0f)
{
	mComponentName = "Launcher";
}

Launcher::~Launcher()
{
}

void Launcher::Init() 
{
}

void Launcher::SelfDelete() 
{
	mTimer = 0.0f;
	tempTime = 0.0f;
	mMissileSpeed = 0.0f;
	mDroneAlive.clear();
	gpComponentManager->mLauncher->Delete(this);
}

void Launcher::Clone(Component * rhs1){
	Launcher* rhs = static_cast<Launcher*>(rhs1);
	 mMissileType = rhs->mMissileType;
	 mTimer = rhs->mTimer;
	 tempTime = rhs->tempTime;
	 mMissileSpeed = rhs->mMissileSpeed;
}

void Launcher::DeleteChildReference(unsigned int id)
{

	//std::vector<Entity *>::iterator itr = std::find(mDroneAlive.begin(), mDroneAlive.end(), );
	//auto it = mDroneAlive.
	Entity * e = nullptr;
	for ( auto d : mDroneAlive ) {
		if (d->id == id)
			e = d;
	}

	std::vector<Entity *>::iterator itr = std::find(mDroneAlive.begin(), mDroneAlive.end(), e);
		
	if (itr != mDroneAlive.end()) {
		mDroneAlive.erase(itr);
	}
}
