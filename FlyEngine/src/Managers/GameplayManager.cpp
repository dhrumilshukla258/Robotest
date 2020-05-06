/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
Author: Ivan Cazares, ivan.c, 60004418
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "GameplayManager.h"

#include "../Entity.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SceneManager.h"
#include "SystemManager.h"
#include "../Components/Body.h"
#include "../Component.h"
#include "../Managers/ObjectFactory.h"
#include "../Systems/PlayerSystem.h"
#include "AudioManager.h"
#include "../Managers/CameraManager.h"

extern EventManager* gpEventManager;
extern EntityManager* gpEntityManager;
extern ComponentManager* gpComponentManager;
extern SceneManager* gpSceneManager;
extern SystemManager* gpSystemManager;
extern AudioManager* gpAudioManager;
extern ObjectFactory* gpObjectFactory;
extern CameraManager* gpCameraManager;
extern bool flip;
#define EVENT_GET(T,TYPE) static_cast<T*>(gpEventManager->GetEvent(TYPE))

GameplayManager::GameplayManager()
{
	gpEventManager->RegisterEvent<Hit,512>(HIT);
}

GameplayManager::~GameplayManager()
{
}

	bool fuseDoor = false;
	bool fusePlatform = false;
	bool fusePlank = false;


	void  GameplayManager::PlayerBulletDestroy(Collider *collider) {
		Entity* pParticleEntity = gpObjectFactory->CreateRuntimeObject("ParticleBulletDestroy.json");//Generate particle
		pParticleEntity->GetComponent<Transform>()->mPos = colPoint;
		gpObjectFactory->DeleteObject(collider->mpOwner->id);
	}

	void  GameplayManager::EnemyBulletDie(Collider *collider) {

		Entity* pParticleEntity = gpObjectFactory->CreateRuntimeObject("ParticleBulletDestroy.json");//
		pParticleEntity->GetComponent<Transform>()->mPos = collider->mpOwner->GetComponent<Transform>()->mPos;
		pParticleEntity->GetComponent<Particle>()->color = glm::vec3(1.0f, 0.0f, 0.0f);
		gpObjectFactory->DeleteObject(collider->mpOwner->id);
		gpAudioManager->Play("EnemyBulletDie.mp3");
	}

	void  GameplayManager::DestroyDrone(Collider *collider) {
		Entity* pFBPtcl = gpObjectFactory->CreateRuntimeObject("ParticleDronDie.json");//create enemy particles
		pFBPtcl->GetComponent<Transform>()->mPos = collider->mpOwner->GetComponent<Transform>()->mPos; // particle in 

		Entity* pFBPtc2 = gpObjectFactory->CreateRuntimeObject("ParticleDronDie2.json");//create enemy particles
		pFBPtc2->GetComponent<Transform>()->mPos = collider->mpOwner->GetComponent<Transform>()->mPos; // particle in 
		Destroy(collider);
		gpAudioManager->Play("DronDie.mp3");
	}

	void  GameplayManager::Destroy(Collider *collider) {
		gpObjectFactory->DeleteObject(collider->mpOwner->id);
	}

	void  GameplayManager::BossBleed(Collider *collider) {
		Entity* pParticleEntity1 = gpObjectFactory->CreateRuntimeObject("ParticleBossBleed1.json");
		Entity* pParticleEntity2 = gpObjectFactory->CreateRuntimeObject("ParticleBossBleed2.json");
		 pParticleEntity1->GetComponent<Transform>()->mPos = collider->mpOwner->GetComponent<Transform>()->mPos;
		 pParticleEntity2->GetComponent<Transform>()->mPos = collider->mpOwner->GetComponent<Transform>()->mPos;
	}


	void  GameplayManager::PlayerBleed(Collider *collider) {
		Entity* pParticleEntity1 = gpObjectFactory->CreateRuntimeObject("ParticlePlayerBleed.json");
		pParticleEntity1->GetComponent<Transform>()->mPos = collider->mpOwner->GetComponent<Transform>()->mPos;
		Entity* pParticleEntity2 = gpObjectFactory->CreateRuntimeObject("ParticlePlayerBleed2.json");
		pParticleEntity2->GetComponent<Transform>()->mPos = collider->mpOwner->GetComponent<Transform>()->mPos;
	}


	//Verify if 2 objects are colliding,  and in wich order are they
	bool GameplayManager::Colliding(EntityType first, EntityType second) {
		_bothPresent = (_type0 == first && _type1 == second) || (_type1 == first && _type0 == second) ? true : false;
		if (_bothPresent) {
			_inOrder = _type0 == first && _type1 == second ? true : false;
		}
		return _bothPresent;
	}

	//Verify if a collider is present in a collision and if is first or second paramether
	bool GameplayManager::OneCollidersTypeIs(EntityType c2) {
		if (_type0 == c2 || _type1 == c2) {
			_inOrder = _type0 == c2 ? true : false;
			return true;
		}
		return false;

	}

	void GameplayManager::ResetTemporals() {
		fuseDoor = false;
	}

	void KeyCollide(Key* k)
	{
		for (auto d : k->mEntity) {
			if (nullptr != d) {
				if (d->mType == DOOR) {
					Door* door = d->GetComponent<Door>();
					door->mKeyHit = true;

					if (!door->mPrevKeyHit && door->mKeyHit) {//press button once
						if (!door->mPrevKeyHit && door->mKeyHit) {//press button once
							gpAudioManager->Play("ButtonPress2.mp3");
							gpAudioManager->Play("DoorOpen.mp3");
						}
						door->mKeyForMultipleHit = !door->mKeyForMultipleHit;
					}
					else if (d->mType == MOVINGPLATFORM) {
						Oscillation* pOsc = d->GetComponent<Oscillation>();
						if (!fusePlatform) {
							fusePlatform = true;
						}

						pOsc->mStart = true;
					}
					else if (d->mType == ROPE) {
						gpObjectFactory->DeleteObject(d->id);
						if (!fusePlank) {
							fusePlank = true;
						}
					}
					else {
							fuseDoor = false;
						}
				}
				else if (d->mType == DRONE) {
					EnemyAI* pEAI = d->GetComponent<EnemyAI>();
					pEAI->mDroneActive = true;
					Homing* pHoming = d->GetComponent<Homing>();
					pHoming->mHomingActive = true;
				}
				
			}
		}
	}

	bool  GameplayManager::CollidersOrder(Contact *c0, Contact *c1) {
		/*EntityType	type0 = c0->mpCollider[0]->mpOwner->mType,
			type1 = c1->mpCollider[1]->mpOwner->mType;
		if(type0)
		return false;*/
		return true;
	}

	void GameplayManager::GenerateBulletExplode(Contact * contact) {//BULLET // MODEL
		Collider *collider = _inOrder ? contact->mpCollider[0] : contact->mpCollider[1];//FIRE_BULLET
		Collider *collider2 = _inOrder ? contact->mpCollider[1] : contact->mpCollider[0];//FOREGROUND

		Entity* pParticleEntity = gpObjectFactory->CreateRuntimeObject("ParticleBulletDestroy.json");//Generate particle
		pParticleEntity->GetComponent<Particle>()->color = collider2->mpOwner->GetComponent<Model>()->mCol;// particle color with  collided color
		pParticleEntity->GetComponent<Transform>()->mPos = colPoint;
		gpObjectFactory->DeleteObject(collider->mpOwner->id);
	}
void GameplayManager::CollisionUpdate(std::list<Contact*>& _contacts)
{
	bool hasPlayerCollided = false;// hack for checking if player collided with anything
	for (auto contact : _contacts) {
		
		Collider *collider;
		_type0 = contact->mpCollider[0]->mpOwner->mType;
		_type1 = contact->mpCollider[1]->mpOwner->mType;
		colPoint = contact->colPoint; 
		
		if (Colliding(FIRE_BULLET, FOREGROUND)) {
			GenerateBulletExplode(contact);
			gpAudioManager->PlaySFX("BulletWall.mp3", 0.4f, 0.6f, -5.0, 5.0f);
			break;
		}

		if (Colliding(FIRE_BULLET, DRONE)) {
			DestroyDrone(_inOrder ? contact->mpCollider[1] : contact->mpCollider[0]);//destroy Drone
			Destroy(_inOrder ? contact->mpCollider[0] : contact->mpCollider[1]);// silently destroy bullet
			break;
		}

		if (Colliding(FIRE_BULLET, LAUNCHER)) {
			collider = _inOrder ? contact->mpCollider[1] : contact->mpCollider[0];//LAUNCHER
			Hit* pH = EVENT_GET(Hit, HIT);
			pH->damage = 50;
			gpEventManager->AddEvent(pH, collider->mpOwner->id);
			Entity* pFBPtcl = gpObjectFactory->CreateRuntimeObject("ParticleTemp.json");//create enemy particles//
			pFBPtcl->GetComponent<Transform>()->mPos = collider->mpOwner->GetComponent<Transform>()->mPos; // particle into enemy 
			Destroy(_inOrder ? contact->mpCollider[0] : contact->mpCollider[1]);//DestroyBullet silently
			gpAudioManager->PlaySFX("DronDie.mp3", 0.4f, 0.7f, 0, 5.0f);
			break;
		}

		if (Colliding(FIRE_BULLET, BOSS)) {
			collider = _inOrder ? contact->mpCollider[1] : contact->mpCollider[0];//BOSS

			Boss* pBoss = collider->mpOwner->GetComponent<Boss>();
			if (pBoss->killedAllLauncher) {
				Hit* pH = EVENT_GET(Hit, HIT);
				pH->damage = 10;
				gpEventManager->AddEvent(pH, collider->mpOwner->id);
				BossBleed(collider);

				gpAudioManager->PlaySFX("BossScream.mp3", 0.6f, 0.8f, -20.0f, 20.0f);
			}

			collider = _inOrder ? contact->mpCollider[0] : contact->mpCollider[1];//FIRE_BULLET
			PlayerBulletDestroy(collider);
			break;
		}

		if (Colliding(FIRE_BULLET, ENEMY)) {
			collider = _inOrder ? contact->mpCollider[1] : contact->mpCollider[0];
			Hit* pH = EVENT_GET(Hit, HIT);
			pH->damage = 100;
			Entity* pFBPtcl = gpObjectFactory->CreateRuntimeObject("ParticleTemp.json");//create enemy particles//
			pFBPtcl->GetComponent<Transform>()->mPos = collider->mpOwner->GetComponent<Transform>()->mPos; // particle into enemy 
			gpEventManager->AddEvent(pH, collider->mpOwner->id);

			collider = _inOrder ? contact->mpCollider[0] : contact->mpCollider[1];
			gpObjectFactory->DeleteObject(collider->mpOwner->id);//Destroy bullet
			gpAudioManager->PlaySFX("GuardDie.mp3", 0.6f, 0.8f, 0, 5.0f);
			break;
		}

		if (Colliding(FIRE_BULLET, MOVINGSPIKES)) {
			GenerateBulletExplode(contact);
			gpAudioManager->PlaySFX("BulletLava.mp3", 0.4f, 0.6f, 0, 5.0f);
			break;
		}

		if (Colliding(FIRE_BULLET, MOVINGPLATFORM)) {
			GenerateBulletExplode(contact);

			gpAudioManager->PlaySFX("BulletWall.mp3", 0.4f, 0.6f, -5.0f, 5.0f);
			break;
		}

		if (Colliding(FIRE_BULLET, DOOR)) {
			GenerateBulletExplode(contact);
			gpAudioManager->PlaySFX("BulletWall.mp3", 0.4f, 0.6f, -5.0f, 5.0f);
			break;
		}

		if (Colliding(FIRE_BULLET, SPIKES)) {
			GenerateBulletExplode(contact);
			gpAudioManager->PlaySFX("BulletLava.mp3", 0.4f, 0.6f, -5.0f, 5.0f);
			break;
		}

		if (Colliding(FIRE_BULLET, PLAYER)) {
			PlayerBulletDestroy(_inOrder ? contact->mpCollider[0] : contact->mpCollider[1]);
			gpAudioManager->PlaySFX("BulletWall.mp3", 0.4f, 0.6f, 0, 5.0f);
			break;
		}

		if (Colliding(FIRE_BULLET, KEY)) {
			PlayerBulletDestroy(_inOrder ? contact->mpCollider[0] : contact->mpCollider[1]);
			collider = _inOrder ? contact->mpCollider[1] : contact->mpCollider[0];
			KeyCollide(collider->mpOwner->GetComponent<Key>());//activates key
			gpAudioManager->PlaySFX("BulletWall.mp3", 0.4f, 0.6f, 0, 5.0f);

			break;
		}

		if (Colliding(ENEMY_BULLET, FOREGROUND)) {
			collider = _inOrder ? contact->mpCollider[0] : contact->mpCollider[1];
			EnemyBulletDie(collider);
		}

		if (Colliding(ENEMY_BULLET, DOOR)) {
			Destroy(_inOrder ? contact->mpCollider[0] : contact->mpCollider[1]);
		}

		if (Colliding(ENEMY_BULLET, MOVINGPLATFORM)) {
			Destroy(_inOrder ? contact->mpCollider[0] : contact->mpCollider[1]);
		}

		if (Colliding(ENEMY_BULLET, SPIKES)) {
			Destroy(_inOrder ? contact->mpCollider[0] : contact->mpCollider[1]);
		}

		if (Colliding(PLAYER, ENEMY_BULLET)) {
			collider = _inOrder ? contact->mpCollider[0] : contact->mpCollider[1];//Player
			Hit* pH = EVENT_GET(Hit, HIT);
			pH->damage = 100;
			gpEventManager->AddEvent(pH, collider->mpOwner->id);//Damages player
			PlayerBleed(collider);

			Destroy(_inOrder ? contact->mpCollider[1] : contact->mpCollider[0]);//Silently destroy bullet
			gpAudioManager->PlaySFX("PlayerHurt.mp3", 0.4f, 0.6f, 0, 5.0f);
		}

		if (Colliding(PLAYER, DRONE)) {
			DestroyDrone(_inOrder ? contact->mpCollider[1] : contact->mpCollider[0]);//Destroy Drone
			collider = _inOrder ? contact->mpCollider[0] : contact->mpCollider[1];//Player 
			Hit* pH = EVENT_GET(Hit, HIT);
			pH->damage = 100;
			gpEventManager->AddEvent(pH, collider->mpOwner->id);//Player gets hurt
			PlayerBleed(collider);

			gpAudioManager->PlaySFX("PlayerHurt.mp3", 0.4f, 0.6f, 0, 5.0f);
		}

		if (Colliding(PLAYER, SPIKES)) {
			collider = _inOrder ? contact->mpCollider[0] : contact->mpCollider[1];//PLAYER
			Hit* pH = EVENT_GET(Hit, HIT);
			pH->damage = 100;
			gpEventManager->AddEvent(pH, collider->mpOwner->id);
		}

		if (Colliding(PLAYER, MOVINGSPIKES)) {
			collider = _inOrder ? contact->mpCollider[0] : contact->mpCollider[1];//PLAYER
			Hit* pH = EVENT_GET(Hit, HIT);
			pH->damage = 100;
			gpEventManager->AddEvent(pH, collider->mpOwner->id);
		}

		if (Colliding(PLAYER, ENEMY_TRIGGER)) {
			collider = _inOrder ? contact->mpCollider[1] : contact->mpCollider[0];//ENEMY_TRIGGER
			collider->mpOwner->mOwnerEntity->GetComponent<EnemyAI>()->SetChasing(true);
			collider->mpOwner->mOwnerEntity->GetComponent<EnemyAI>()->SetPatroling(false);
		}

		if (Colliding(PLAYER, TEXT_TRIGGER)) {
			collider = _inOrder ? contact->mpCollider[1] : contact->mpCollider[0];//TEXT_TRIGGER
			collider->mpOwner->mOwnerEntity->GetComponent<Text>()->mTextAppear = true;
		}

		if (Colliding(PLAYER, DRONE_TRIGEGR)) {
			collider = _inOrder ? contact->mpCollider[1] : contact->mpCollider[0];//DRONE_TRIGEGR
			collider->mpOwner->mOwnerEntity->GetComponent<EnemyAI>()->mDroneActive = true;
			collider->mpOwner->mOwnerEntity->GetComponent<Homing>()->mHomingActive = true;
		}

		if (Colliding(PLAYER, CAMERA_TRIGGER)) {
			collider = _inOrder ? contact->mpCollider[1] : contact->mpCollider[0];//CAMERA_TRIGGER
			gpCameraManager->SetBounds(
				collider->mpOwner->GetComponent<Camera>()->mMinBound,
				collider->mpOwner->GetComponent<Camera>()->mMaxBound
			);
			gpCameraManager->keepFollowing = collider->mpOwner->GetComponent<Camera>()->keepFollow;
		}

		if (Colliding(PLAYER, DOOR_CLOSE_TRIGGER)) {
			collider = _inOrder ? contact->mpCollider[1] : contact->mpCollider[0];// DOOR_CLOSE_TRIGGER
			Entity* mParentDoor = collider->mpOwner->mParent;
			Door* pDoor = mParentDoor->GetComponent<Door>();
			pDoor->isOpen = true;
			pDoor->closeDoor = true;
		}

		if (Colliding(PLAYER, CHECKPOINT)) {
			collider = _inOrder ? contact->mpCollider[1] : contact->mpCollider[0];// CHECKPOINT
			gpSystemManager->mPlayerSystem->mCheckpoint = collider->mpTr->mPos;	
		}


		if (OneCollidersTypeIs(KEY)) {
			collider = _inOrder ? contact->mpCollider[0] : contact->mpCollider[1];//KEY
			KeyCollide(collider->mpOwner->GetComponent<Key>());
		}


//------------------------------------------------------------------------------------------------------------
		
		EntityType	type0 = contact->mpCollider[0]->mpOwner->mType, 
					type1 = contact->mpCollider[1]->mpOwner->mType;

		if (type0 == FIRE_BULLET) {
			if (type1 != CHECKPOINT) {
				//Bullet Deletion
				Entity* pBullet = contact->mpCollider[0]->mpOwner;
				Entity* pParticleEntity = gpObjectFactory->CreateRuntimeObject("ParticleBulletDestroy.json");

				Transform* pBtTr = pBullet->GetComponent<Transform>();
				Body* pBtBody = pBullet->GetComponent<Body>();

				Transform* pPtTr = pParticleEntity->GetComponent<Transform>();
				Particle* pPtPtcl = pParticleEntity->GetComponent<Particle>();

				pPtTr->mPos = pBtTr->mPos;
				pPtPtcl->position_adjust *= glm::normalize(glm::vec3(pBtBody->mVel, 0.0f));

				gpObjectFactory->DeleteObject(contact->mpCollider[0]->mpOwner->id);
			}
			continue;
		}		
		else if (type1 == FIRE_BULLET) {
		
			if (type0 != CHECKPOINT) {
				//Bullet Deletion
				Entity* pParticleEntity = gpObjectFactory->CreateRuntimeObject("ParticleBulletDestroy.json");
				pParticleEntity->GetComponent<Transform>()->mPos = contact->mpCollider[0]->mpOwner->GetComponent<Transform>()->mPos;
				gpObjectFactory->DeleteObject(contact->mpCollider[1]->mpOwner->id);
			}
			continue;
		}
	
	//	if (type1 == KEY) {
	//		KeyCollide(contact->mpCollider[1]->mpOwner->GetComponent<Key>());
	////	gpAudioManager->Play("PlayerPressSwitch.wav");
	//	}
	//	else if (type0 == KEY) {
	//		KeyCollide(contact->mpCollider[0]->mpOwner->GetComponent<Key>());
	//	//	gpAudioManager->Play("PlayerPressSwitch.wav");
	//	}


		// ENEMY WAY POINTS
		if (type0 == ENEMY_WAY_POINT) {
			if (type1 == ENEMY) {
				if (contact->mpCollider[0]->mpOwner->mOwnerEntity->GetComponent<EnemyAI>()->GetPatrolingState())
					contact->mpCollider[0]->mpOwner->mOwnerEntity->GetComponent<EnemyAI>()->ResolveWayPoint(contact->mpCollider[0]->mpOwner);
			}
			if (type1 == PLAYER) {
				int x;
				x = 0;
			}
		}
		else if (type1 == ENEMY_WAY_POINT) {
			if (type0 == ENEMY) {
				if (contact->mpCollider[1]->mpOwner->mOwnerEntity->GetComponent<EnemyAI>()->GetPatrolingState())
					contact->mpCollider[1]->mpOwner->mOwnerEntity->GetComponent<EnemyAI>()->ResolveWayPoint(contact->mpCollider[1]->mpOwner);
			}
			if (type0 == PLAYER) {
				int x;
				x = 0;
			}
		}

		//if (type0 == CHECKPOINT) {
		//	if (type1 == PLAYER) {
		//		gpSystemManager->mPlayerSystem->mCheckpoint = contact->mpCollider[0]->mpTr->mPos;
		//	}
		//}
		//else if (type1 == CHECKPOINT) {
		//	if (type0 == PLAYER) {
		//		gpSystemManager->mPlayerSystem->mCheckpoint = contact->mpCollider[0]->mpTr->mPos;
		//	}
		//}

		// END
		if (type0 == END) {
			if (type1 == PLAYER) {
				gpSceneManager->LoadNextLevel();
			}
		}
		else if (type1 == END) {
			if (type0 == PLAYER) {
				gpSceneManager->LoadNextLevel();
			}
		}
		
		if (type0 == PLAYER) {
			if (contact->mpCollider[1]->isTrigger) {
				//gpSystemManager->mPlayerSystem->mCheckpoint = contact->mpCollider[1]->mpTr->mPos;
				if (type1 == BOX) {
					gpSystemManager->mPlayerSystem->grabbingState.grabbedBody = contact->B;
				}
				
			}
			if (contact->colNormal == glm::vec2(0, -1) && !contact->mpCollider[1]->isTrigger) {
					hasPlayerCollided = true;
				if (gpSystemManager->mPlayerSystem->jumpState.jumpingDown ||
					gpSystemManager->mPlayerSystem->fallState.isFalling)
					gpSystemManager->mPlayerSystem->isGrounded = true;
			}
		}
		else if (type1 == PLAYER) {
			if (contact->mpCollider[0]->isTrigger) {
				//gpSystemManager->mPlayerSystem->mCheckpoint = contact->mpCollider[1]->mpTr->mPos;
				if (type0 == BOX) {
					gpSystemManager->mPlayerSystem->grabbingState.grabbedBody = contact->A;
				}
				
			}
			if (contact->colNormal == glm::vec2(0, 1) && !contact->mpCollider[0]->isTrigger) {
				hasPlayerCollided = true;
				if (gpSystemManager->mPlayerSystem->jumpState.jumpingDown ||
					gpSystemManager->mPlayerSystem->fallState.isFalling)
					gpSystemManager->mPlayerSystem->isGrounded = true;
			}
		} 
	}
		// this works on the logic that if the player didn't collide with
		// anything this frame then it was in the air (not grounded)
		if (!hasPlayerCollided) {
		//		gpSystemManager->mPlayerSystem->pBody->isConstantVelocitySet = false;
			
			if (gpSystemManager->mPlayerSystem->jumpState.jumpingUp
				|| gpSystemManager->mPlayerSystem->jumpState.jumpingDown ||
				gpSystemManager->mPlayerSystem->fallState.isFalling) {
				gpSystemManager->mPlayerSystem->isGrounded = false;
			}
		}
}

void GameplayManager::HandleEvent(Event* _pEvent, unsigned int _id)
{
	//...This function is called from eventManager
	//...Any System which wants some events to be added will
	//...call EventManager and add that event and the id of the entity
	//...for which the event is related to.
	_pEvent->Update(_id);
}

void Hit::Update(unsigned int _id)
{
	Entity* en = gpEntityManager->mEntity[_id];
	Health* pH = en->GetComponent<Health>();
	pH->mHealthLeft -= damage;
	//std::cout << "Hit Event Called Damage Done" << _id << "\n" ;
}

