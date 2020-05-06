/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
Author: Ivan Cazares, ivan.c, 60004418
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#pragma once
#include "CollisionManager.h"
#include "EventManager.h"
#include "../Entity.h"

class Event;
class GameplayManager
{
public:
	GameplayManager();
	~GameplayManager();
	void CollisionUpdate(std::list<Contact*>&);
	bool CollidersOrder(Contact *c0, Contact *c1);
	void HandleEvent(Event* , unsigned int);
	void ResetTemporals();
	void PlayerBulletDestroy(Collider *collider);
	void EnemyBulletDie(Collider *collider);
	void  DestroyDrone(Collider *collider);
	/// Locates and destroy entity of that collider
	void Destroy(Collider *collider);
	void GenerateBulletExplode(Contact *contact);
	void BossBleed(Collider *collider);
	void PlayerBleed(Collider *collider);
	bool Colliding(EntityType first, EntityType second);
	bool OneCollidersTypeIs(EntityType c2);
public:
	EntityType	_type0, _type1;// we will make so much comparsions.  of type from 2 entities this is the holder 
	bool _bothPresent;// if both types are present, then true (order is not compulsery)
	bool _inOrder;// if the list of paramehers came in order , this is true
	glm::vec3 colPoint;
};

//Describe all type of Events Here
class Hit : public Event {
public:
	Hit() : Event(HIT), damage(1) {}
	Hit* GetSelf() { return this; }
	void Update(unsigned int);
	
	~Hit() {}
public:
	int damage;

	
};
