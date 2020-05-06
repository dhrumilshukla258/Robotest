#pragma once

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();
	void Update();
public:
	float timer_count;
	bool start_Stop;
};