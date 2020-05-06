/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "ComponentManager.h"
#include "../Components/Shape.h"
#include "../Utilities/ShapeStructure.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<ComponentManager>("ComponentManager")
		 .constructor()
		 .property("mTransform", &ComponentManager::mTransform)
		 .property("mSprite", &ComponentManager::mSprite)
		 .property("mCamera", &ComponentManager::mCamera)
		 .property("mControl", &ComponentManager::mControl)
		 .property("mHealth", &ComponentManager::mHealth)
		 .property("mTimer", &ComponentManager::mTimer)
		 .property("mRawModel", &ComponentManager::mRawModel)
		 .property("mStencil_Toon_Model", &ComponentManager::mStencil_Toon_Model)
	     .property("mParticle", &ComponentManager::mParticle)
		 .property("mParticleGenerator", &ComponentManager::mParticleGenerator)
		 .property("mModel", &ComponentManager::mModel)
		 .property("mCollider", &ComponentManager::mCollider)
		 .property("mBackgroundModel", &ComponentManager::mBackground_Model)
		 .property("mBody", &ComponentManager::mBody)
		 .property("mOscillation", &ComponentManager::mOscillation)
		 .property("mKey", &ComponentManager::mKey)
		 .property("mScript", &ComponentManager::mScript)
		 .property("mDoor", &ComponentManager::mDoor)
		 .property("mLauncher", &ComponentManager::mLauncher)
		 .property("mHoming", &ComponentManager::mHoming)
		 .property("mLight", &ComponentManager::mLight)
		 .property("mWorld", &ComponentManager::mWorld)
		 .property("mEnemyAI", &ComponentManager::mEnemyAI)
		 .property("mAnimation", &ComponentManager::mAnimation)
		 .property("mText", &ComponentManager::mText)
		 .property("mBoss", &ComponentManager::mBoss)
	
		 .method("ComponentConstructor", &ComponentManager::ComponentConstructor)
		 .method("ComponentDestructor", &ComponentManager::ComponentDestructor)
		 .method("DeleteComponent", &ComponentManager::DeleteComponent)
		 .method("Init", &ComponentManager::Init);
}

ComponentManager::ComponentManager()
{

	invoke("ComponentConstructor");
	mShapeAABB = new ShapeStructure;
	mShapeCircle = new ShapeStructure;

	mTransform->generatePool<Transform,1024>();
	mSprite->generatePool<Sprite,1024>();
	mModel->generatePool<Model,1024>();
	mRawModel->generatePool<RawModel, 1024>();
	mStencil_Toon_Model->generatePool<Stencil_Toon_Model, 1024>();
	mBackground_Model->generatePool<BackgroundModel, 1024>();
	mBody->generatePool<Body,1024>();
	mHealth->generatePool<Health, 1024>();
	mTimer->generatePool<Timer, 1024>();
	mParticle->generatePool<Particle, 18096>();
	mParticleGenerator->generatePool<ParticleGenerator, 18096>();
	mOscillation->generatePool<Oscillation, 128>();
	mCamera->generatePool<Camera, 8>();
	mControl->generatePool<Control, 16>();
	mKey->generatePool<Key, 64>();
	mCollider->generatePool<Collider, 1024>();
	mScript->generatePool<Script, 1024>();
	mDoor->generatePool<Door, 64>();
	mHoming->generatePool<Homing, 512>();
	mLauncher->generatePool<Launcher, 128>();
	mEnemyAI->generatePool<EnemyAI, 128>();
	

	mShapeAABB->generatePool<ShapeAABB,1024>();
	mShapeCircle->generatePool<ShapeCircle,1024>();
	mLight->generatePool<Light, 1024>();
	mWorld->generatePool<World, 1024>();
	mAnimation->generatePool<Animation, 1024>();
	mText->generatePool<Text, 512>();

	mBoss->generatePool<Boss, 8>();
}

ComponentManager::~ComponentManager()
{
	invoke("ComponentDestructor"); 
	delete mShapeAABB;
	delete mShapeCircle;
}

void ComponentManager::invoke(std::string _method, unsigned int id)
{
	rttr::type component_pool = rttr::type::get_by_name("ComponentManager");
	rttr::method meth = component_pool.get_method(_method);
	if (meth) // check if the function was found
	{
		rttr::instance ol = *this;
		if (ol.is_valid()) {
			for (auto prop : component_pool.get_properties()) {
				rttr::variant var = prop.get_value(ol);

				if (var.can_convert<MapListStructure*>()) {

					MapListStructure* pMLS = var.convert<MapListStructure*>();
					if (_method == "ComponentConstructor" || _method == "ComponentDestructor" || _method == "Init") {
						
						rttr::variant return_value = meth.invoke(ol, { pMLS });
						
						if (!(return_value.is_valid()))
							std::cout << "Errorn in invoking " << meth.get_name().to_string() << std::endl;
						
						else if (return_value.is_valid() && return_value.is_type<MapListStructure*>())
							prop.set_value(ol, return_value.convert<MapListStructure*>());
						
					}

					else if (_method == "DeleteComponent" || _method == "Init_ID") {
						rttr::variant return_value = meth.invoke(ol, pMLS, id );
					}
				}
			}
		}
	}
}

MapListStructure* ComponentManager::ComponentConstructor(MapListStructure* pMLS)	{ return new MapListStructure(); }

void ComponentManager::ComponentDestructor(MapListStructure* pMLS)					{ delete pMLS; }

void ComponentManager::Init(MapListStructure* pMLS)									
{
	Component* c = pMLS->GetHead();
	while (nullptr != c) {
		c->Init();
		c = c->GetNext();
	}
}

void ComponentManager::DeleteComponent(MapListStructure* pMLS, unsigned int _id)
{
	mShapeAABB->Delete(_id);
	mShapeCircle->Delete(_id);
}