#pragma once

#include "../Component.h"

class Sprite : public Component
{
public:
	Sprite();
	~Sprite();

	void Init();
	void SelfDelete();
	void Clone(Component *);

public:
	std::string texturePath;
	std::string shape;
	unsigned int mVAO;
	unsigned int mTextureID;

private:
	RTTR_ENABLE(Component)
};