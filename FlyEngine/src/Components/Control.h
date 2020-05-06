#pragma once
#include "../Component.h"

class Control : public Component
{
public:
	Control();
	~Control();
	void Init() {}
	void SelfDelete();
	void Clone(Component *);

};