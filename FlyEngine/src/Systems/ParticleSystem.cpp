/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/


#include "../pch.h"
#include "ParticleSystem.h"
#include "../Managers/ComponentManager.h"
#include "../Components/ParticleGenerator.h"
#include "../Components/Particle.h"
#include "../Managers/FrameRateController.h"
#include "../Managers/CameraManager.h"
#include "../Managers/SystemManager.h"
#include "PlayerSystem.h"

#include <random>

std::mt19937_64 RNGen1;
std::uniform_real_distribution<> myrandom1(0.0f, 1.0f);

extern ComponentManager* gpComponentManager;
extern FrameRateController* gpFrameRateController;
extern CameraManager* gpCameraManager;
extern 	SystemManager*   gpSystemManager;

#define  Player_Particle_Distance 15000.0f

///////////////////
ParticleSystem::ParticleSystem():timer_count(0)
{
}

ParticleSystem::~ParticleSystem()
{
}

float Distance_Chec(glm::vec3 A, glm::vec3 B)
{
	return fabs((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y) + (A.z - B.z)*(A.z - B.z));
}
bool Life_Check(Particle_Struct* temp)
{
	if (temp->life < 0.0f||temp->color_W<0.0f||temp->scale.x<0.0f || temp->scale.y < 0.0f || temp->scale.z < 0.0f)
	{
		temp->~Particle_Struct();
		return true;
	}
	else
	{

		return false;
	}
	/*bool x = temp->life < 0.0f;
	if (x) {
		temp->SelfDelete();
	}
	return x;*/
}

void Particle_Update(Particle_Struct* temp)
{
	//temp->color -= temp->color;
	temp->color_W -= temp->color_fade;
	temp->life -= temp->kill_time;
	temp->position += temp->velocity*temp->velocity_control;
	temp->scale.x = temp->scale.x + temp->x_scale_adjust;
	temp->scale.y = temp->scale.y + temp->y_scale_adjust;
	temp->scale.z = temp->scale.z + temp->z_scale_adjust;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, temp->position);

	model = glm::scale(model, temp->scale);
	*temp->model_particle = model;
}

float RandomFloat(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}

void ParticleSystem::Update()
{
	ParticleGenerator* pPartGen;
	Particle* pPart;
	Body *pBody;
	Transform *pTr;
	Transform *pTr_Player;
	int number_particles_Max = 2;

	PlayerSystem* mPlayer_system_temp = gpSystemManager->mPlayerSystem;
	if (mPlayer_system_temp->GetPlayerEntity() == nullptr)
	{
		return;
	}
	pTr_Player = mPlayer_system_temp->GetPlayerEntity()->GetComponent<Transform>();

	Component* c = gpComponentManager->mParticleGenerator->GetHead();
	while (nullptr != c) {
		pPartGen = static_cast<ParticleGenerator*>(c);
		pTr = pPartGen->mpOwner->GetComponent<Transform>();
		pPart = pPartGen->mpOwner->GetComponent<Particle>();

		////////
		//pPartGen->color_Particles.clear();
		//pPartGen->model_Matrix.clear();
		//pPartGen->particle_list.clear();
		

		//////
		//if (Distance_Chec(pTr_Player->mPos, pTr->mPos) > Player_Particle_Distance)
		//{
		//	pPartGen->color_Particles.clear();
		//	pPartGen->model_Matrix.clear();
		//	pPartGen->particle_list.clear();
		//	c = c->GetNext();
		//	continue;
		//}

		if (pPart == NULL)
		{
			c = c->GetNext();
			continue;
		}
		if (pPart->particle_enable == false)
		{
			if (pPartGen->particle_list.size() != 0)
			{
				pPartGen->model_Matrix.clear();
				pPartGen->color_Particles.clear();
				pPartGen->particle_list.remove_if(Life_Check);
				std::for_each(pPartGen->particle_list.begin(), pPartGen->particle_list.end(), Particle_Update);
				std::list<Particle_Struct*>::iterator it;
				for (it = pPartGen->particle_list.begin(); it != pPartGen->particle_list.end(); ++it)
				{
					pPartGen->model_Matrix.push_back(*(*it)->model_particle);
					glm::vec4 temp_color = glm::vec4((*it)->color.x, (*it)->color.y, (*it)->color.z, (*it)->color_W);
					pPartGen->color_Particles.push_back(temp_color);
				}
			}
			
			c = c->GetNext();
			continue;
		}

		pPartGen->model_Matrix.clear();
		pPartGen->color_Particles.clear();
		
		pPartGen->mTextureID = pPart->texture_ID[pPart->current_texture_ID];
		timer_count += (float) gpFrameRateController->GetFrameTime();
		
		if (pPart->static_dynamic == 1)
		{
			pBody = pPartGen->mpOwner->GetComponent<Body>();
			if (pBody == NULL)
				continue;
			//Player:  foot dust only if player is Running 
			if (  fabs(pBody->mVel.x) > 21.0f){
				for (int i = 0; i < pPart->max_Paticles; ++i)
				{
					Particle_Struct* temp= new Particle_Struct();
					//Particle* temp = static_cast<Particle*> (gpComponentManager->mParticle->Add(0));
					//temp->Initialize(pPart->type);
					temp->color = pPart->color;
					temp->color_fade = pPart->color_fade;
					temp->color_W = pPart->color_W;
					temp->kill_time = pPart->kill_time;
					temp->life = pPart->life;
					temp->max_Paticles = pPart->max_Paticles;
					//temp->model_particle = pPart->model_particle;
					temp->position = pPart->position;
					temp->position_adjust = pPart->position_adjust;
					temp->scale = pPart->scale;
					temp->static_dynamic = pPart->static_dynamic;
					
					temp->velocity = pPart->velocity ;
					temp->velocity_adjust = pPart->velocity_adjust;
					temp->velocity_control = pPart->velocity_control;
					temp->x_axis_adjust = pPart->x_axis_adjust;
					temp->y_axis_adjust = pPart->y_axis_adjust;
					temp->z_axis_adjust = pPart->z_axis_adjust;
					temp->x_scale_adjust = pPart->x_scale_adjust;
					temp->y_scale_adjust = pPart->y_scale_adjust;
 					temp->z_scale_adjust = pPart->z_scale_adjust;
					//Particle* temp = new Particle();
					//pPart->Initialize()
					//temp->Initialize(pPart->type);
					temp->position = glm::vec3(pBody->mPos.x +
						RandomFloat(-temp->x_axis_adjust, temp->x_axis_adjust),
						pBody->mPos.y +
						RandomFloat(-temp->y_axis_adjust, temp->y_axis_adjust), +
						RandomFloat(-temp->z_axis_adjust, temp->z_axis_adjust)) +
						temp->position_adjust;
							/*temp->position = glm::vec3(pBody->mPos.x +
								RandomFloat(-temp->x_axis_adjust, temp->x_axis_adjust),
								pBody->mPos.y +
								RandomFloat(-temp->y_axis_adjust, temp->y_axis_adjust), +
								RandomFloat(-temp->z_axis_adjust, temp->z_axis_adjust)) +
								temp->position_adjust;*/

							temp->velocity = glm::vec3(pBody->mVel.x, pBody->mVel.y, 0.0f) + temp->velocity_adjust;
							*temp->model_particle = glm::mat4(1.0f);
							pPartGen->particle_list.push_back(temp);


				}
			}
		}
		else
		{
			pTr = pPartGen->mpOwner->GetComponent<Transform>();
			if (pTr == NULL)
				continue;
			for (int i = 0; i < pPart->max_Paticles; ++i)
			{
				Particle_Struct* temp = new Particle_Struct();
				temp->color = pPart->color;
				temp->color_fade = pPart->color_fade;
				temp->color_W = pPart->color_W;
				temp->kill_time = pPart->kill_time;
				temp->life = pPart->life;
				temp->max_Paticles = pPart->max_Paticles;
				temp->position = pPart->position;
				temp->position_adjust = pPart->position_adjust;
				temp->scale = pPart->scale;
				temp->static_dynamic = pPart->static_dynamic;
				temp->velocity = pPart->velocity;

				////TEST to add some randomness to the particles by Ivan Cazares 20190410
				temp->velocity.x = pPart->velocity.x + (-0.5f + (float)myrandom1(RNGen1));
				temp->velocity.y = pPart->velocity.y + (-0.5f + (float)myrandom1(RNGen1));


				temp->velocity_adjust = pPart->velocity_adjust;
				temp->velocity_control = pPart->velocity_control;
				temp->x_axis_adjust = pPart->x_axis_adjust;
				temp->y_axis_adjust = pPart->y_axis_adjust;
				temp->z_axis_adjust = pPart->z_axis_adjust;
				temp->x_scale_adjust = pPart->x_scale_adjust;
				temp->y_scale_adjust = pPart->y_scale_adjust;
				temp->z_scale_adjust = pPart->z_scale_adjust;
				temp->position = glm::vec3(pTr->mPos.x +
						RandomFloat(-temp->x_axis_adjust, temp->x_axis_adjust),
						pTr->mPos.y +
						RandomFloat(-temp->y_axis_adjust, temp->y_axis_adjust), RandomFloat(-temp->z_axis_adjust, temp->z_axis_adjust));

				temp->position = temp->position + temp->position_adjust;
				/*	if (temp->velocity_adjust.y < 0)
					{
						temp->velocity = glm::vec3(RandomFloat(-temp->velocity_adjust.x, temp->velocity_adjust.x), temp->velocity_adjust.y, RandomFloat(-temp->velocity_adjust.z, temp->velocity_adjust.z));

					}
					else
					{
						temp->velocity = glm::vec3(RandomFloat(-temp->velocity_adjust.x, temp->velocity_adjust.x), RandomFloat(-temp->velocity_adjust.y, temp->velocity_adjust.y), RandomFloat(-temp->velocity_adjust.z, temp->velocity_adjust.z));

					}*/
					*temp->model_particle = glm::mat4(1.0f);
					pPartGen->particle_list.push_back(temp);
			}
		}
		
		
		if (pPartGen->particle_list.size() != 0)
		{
			pPartGen->particle_list.remove_if(Life_Check);
			std::for_each(pPartGen->particle_list.begin(), pPartGen->particle_list.end(), Particle_Update);
		}

		std::list<Particle_Struct*>::iterator it;
		for (it = pPartGen->particle_list.begin(); it != pPartGen->particle_list.end(); ++it)
		{

			pPartGen->model_Matrix.push_back(*(*it)->model_particle);
			glm::vec4 temp_color = glm::vec4((*it)->color.x, (*it)->color.y, (*it)->color.z, (*it)->color_W);
			pPartGen->color_Particles.push_back(temp_color);
		}

		c = c->GetNext();
	}
}