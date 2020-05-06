#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H
#include "../pch.h"
#include "../Component.h"

struct Particle_Struct
{
public:

	glm::vec3 position, velocity, position_adjust, scale, velocity_adjust;
	glm::vec3 color;
	float color_W;
	float life;
	float kill_time;
	float velocity_control;
	float color_fade;
	float x_axis_adjust, y_axis_adjust, z_axis_adjust;
	float x_scale_adjust, y_scale_adjust, z_scale_adjust;

	int max_Paticles;
	glm::mat4 *model_particle;
	std::string particleFile;
	int static_dynamic;
	bool timer;
	int timer_Limit;
	
public:
	Particle_Struct()
	{
		model_particle = reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16));
	}
	~Particle_Struct()
	{
		_aligned_free(model_particle);
	}
};
enum Particle_Type {
	TYRE_SMOKE,
	HOMING_MISSILE,
	BULLET_FIRE,
	BLAST_EFFECT_1,
	GUN_FIRE_PARTICLES,
	FIRE_FLAME
};

class Particle : public Component
{
public:
	Particle();
	//Particle(int _type);
	~Particle();
	void Init();	
	void Initialize(int _type);
	void SelfDelete();
	void Clone(Component *);

public:
	int type;



	glm::vec3 position, velocity, position_adjust, scale, velocity_adjust;
	glm::vec3 color;
	float color_W;
	float life;
	float kill_time;
	float velocity_control;
	float color_fade;
	float x_axis_adjust, y_axis_adjust, z_axis_adjust;
	float x_scale_adjust, y_scale_adjust, z_scale_adjust;
	
	int max_Paticles;
	glm::mat4* model_particle = nullptr;
	std::string particleFile;
	int static_dynamic;
	bool timer;
	int timer_Limit;
	
	bool particle_enable;

	int texture_count=4;
	std::string  texture_name[4] = { 
		"ParticleBlast.png",
		"ParticleFog.png",
		"ParticleFire.png",
		"ParticleSparcle.png" 
	};
	//,
	//	"Media_Militia_Particles001.png",
	//	"Media_Militia_Particles002.png",
	//	"Media_Militia_Particles003.png",
	//	"Media_Militia_Particles004.png",
	//	"Media_Militia_Particles005.png",
	//	"Media_Militia_Particles006.png",
	//	"Media_Militia_Particles007.png",
	//	"Media_Militia_Particles008.png",
	//	"Media_Militia_Particles009.png",
	//	"Media_Militia_Particles010.png",
	//	"Media_Militia_Particles011.png",
	//	"Media_Militia_Particles012.png",
	//	"Media_Militia_Particles013.png",
	//	"Media_Militia_Particles014.png",
	//	"Media_Militia_Particles015.png",
	//	"Media_Militia_Particles016.png",
	//	"Media_Militia_Particles017.png",
	//	"Media_Militia_Particles018.png",
	//	"Media_Militia_Particles019.png",
	//	"Media_Militia_Particles020.png",
	//	"Media_Militia_Particles021.png",
	//	"Media_Militia_Particles022.png",
	//	"Media_Militia_Particles023.png",
	//	"Media_Militia_Particles024.png",
	//	"Media_Militia_Particles025.png",
	//};
	std::vector<unsigned int> texture_ID;
int current_texture_ID = 1;


private:
	RTTR_ENABLE(Component)
};

#endif