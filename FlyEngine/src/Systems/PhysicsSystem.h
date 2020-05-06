#pragma once
class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	glm::vec2 gravity;
	float fixedDeltaTime;
	void ResetForces();
	void Update(float);

	void InterpolateState(float);
	

};

