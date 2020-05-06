#pragma once
#include "../Component.h"

class Key : public Component
{
public:
	Key();
	~Key();
	void Init();
	void SelfDelete();
	void Clone(Component * );
	void DeleteChildReference(unsigned int);

public:
	std::vector<Entity*> mEntity;
private:
	RTTR_ENABLE(Component)
};