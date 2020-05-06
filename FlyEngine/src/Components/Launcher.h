#pragma once
#include "../Component.h"

enum MissileType {
	ShootBullet,
	ShootHoming,
	Drone
};

class Launcher : public Component
{
public:
	Launcher();
	~Launcher();
	void Init();
	void SelfDelete();
	void Clone(Component *);
	void DeleteChildReference(unsigned int);
public:
	MissileType mMissileType;
	float mTimer;
	float tempTime;
	float mMissileSpeed;
	std::vector<Entity*> mDroneAlive;
	int bossSystemNeedsThis = 0;
private:
	RTTR_ENABLE(Component)
};