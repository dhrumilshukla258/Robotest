#pragma once
class HealthSystem
{
public:
	HealthSystem();
	~HealthSystem();
	void Update();

	float respawnTime = 1.5f;
	float timeCounter = 0.0f;
	bool diedRecently = false;
	float immortalityTime = 0.5f;
};
