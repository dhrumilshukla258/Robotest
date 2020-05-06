/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Particle.h"
#include "../Managers/GraphicsManager.h"
#include "../Managers/ResourceManager.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

//using namespace rttr;
extern GraphicsManager* gpGraphicsManager;
extern ResourceManager * gpResourceManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<Particle>("Particle")

	.property("Type", &Particle::type)

	.property("Static_Dynamic", &Particle::static_dynamic)
	.property("Position", &Particle::position)
	.property("Velocity", &Particle::velocity)
	.property("Position_Adjust", &Particle::position_adjust)
	.property("Color", &Particle::color)
	.property("Color_W", &Particle::color_W)
	.property("Life", &Particle::life)
	.property("Kill_Time", &Particle::kill_time)
	.property("Velocity_Control", &Particle::velocity_control)
	.property("Color_Fade", &Particle::color_fade)
	.property("X_Axis_Adjust", &Particle::x_axis_adjust)
	.property("Y_Axis_Adjust", &Particle::y_axis_adjust)
	.property("Z_Axis_Adjust", &Particle::z_axis_adjust)
	.property("X_Scale_Adjust", &Particle::x_scale_adjust)
	.property("Y_Scale_Adjust", &Particle::y_scale_adjust)
	.property("Z_Scale_Adjust", &Particle::z_scale_adjust)
	.property("Scale", &Particle::scale)
	.property("Max_Paticles", &Particle::max_Paticles)
	.property("Current_Texture_ID", &Particle::current_texture_ID);



}

Particle::Particle() 
{
	//color = reinterpret_cast<glm::vec4*> (_aligned_malloc(sizeof(glm::vec4), 16));
	model_particle = reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16));
	mComponentName = "Particle";
	particle_enable = true;
}

Particle::~Particle()
{
	_aligned_free(model_particle);
	//_aligned_free(color);
}

void Particle::Init()
{
	
texture_name[0] = { "ParticleBlast.png" };
texture_name[1] = { "ParticleFog.png" };
texture_name[2] = { "ParticleFire.png" };
texture_name[3] = { "ParticleSparcle.png" };
for (int i = 0;i < texture_count;++i)
	{
		texture_ID.push_back(gpResourceManager->LoadTexture(texture_name[i]));
	}
}

void Particle::Initialize(int _type) 
{
	if (_type == TYRE_SMOKE)
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		position_adjust = glm::vec3(0.0f, -2.0f, 0.0f);
		velocity_adjust = glm::vec3(0.0f, 0.0f, 0.0f);
		//(*color) = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		life = 1.0f;
		kill_time = 0.03f;
		velocity_control = 0.001f;
		color_fade = 0.05f;
		x_axis_adjust = 1.0f;
		y_axis_adjust = 1.0f;
		z_axis_adjust = 1.0f;
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		max_Paticles = 2;
		current_texture_ID = 1;
		particle_enable = true;
		
	}
	else 
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		position_adjust = glm::vec3(0.0f, -2.0f, 0.0f);
		velocity_adjust = glm::vec3(0.0f, 0.0f, 0.0f);
		//(*color) = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		life = 1.0f;
		kill_time = 0.03f;
		velocity_control = 0.001f;
		color_fade = 0.05f;
		x_axis_adjust = 1.0f;
		y_axis_adjust = 1.0f;
		z_axis_adjust = 1.0f;
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		max_Paticles = 2;
		
		current_texture_ID = 1;
		particle_enable = true;
	}
	

	for (int i = 0;i < 1;++i)
	{
		texture_ID.push_back(gpResourceManager->LoadTexture(texture_name[i]));
	}

	
}

void Particle::SelfDelete() 
{

	static_dynamic = 0;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	position_adjust = glm::vec3(0.0f, -2.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	velocity_adjust = glm::vec3(0.0f, 0.0f, 0.0f);
	//(*color) = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	life = 1.0f;
	kill_time = 0.03f;
	velocity_control = 0.001f;
	color_fade = 0.05f;
	x_axis_adjust = 1.0f;
	y_axis_adjust = 1.0f;
	z_axis_adjust = 1.0f;
	max_Paticles = 1;
	*model_particle = glm::mat4(0.0f);
	particleFile = "";
	current_texture_ID = 1;
	particle_enable = true;
	gpComponentManager->mParticle->Delete(this);
}

void Particle::Clone(Component *rhs1) {
	Particle* rhs = static_cast<Particle*>(rhs1);

	type = rhs->type;
	static_dynamic = rhs->static_dynamic;
	position = rhs->position;
	velocity = rhs->velocity;
	position_adjust = rhs->position_adjust;
	scale = rhs->scale;
	velocity_adjust = rhs->velocity_adjust;
	//color = nullptr;
	life = rhs->life;;
	kill_time = rhs->kill_time;
	velocity_control = rhs->velocity_control;
	color_fade = rhs->color_fade;
	x_axis_adjust = rhs->x_axis_adjust;
	y_axis_adjust = rhs->y_axis_adjust;
	z_axis_adjust = rhs->z_axis_adjust;
	max_Paticles = rhs->max_Paticles;
	//model_particle = nullptr;
	particleFile = rhs->particleFile;
	current_texture_ID = rhs->current_texture_ID;
}