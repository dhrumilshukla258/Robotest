#pragma once

#include "../Component.h"

class Text : public Component
{
public:
	Text();
	~Text();

	void Init();
	void SelfReset();
	void Clone(Text *);
	void SelfDelete();
	void DeleteChildReference(unsigned int);

public:
	std::string mStr;
	std::string mJoyStickStr;
	Entity* mTextCollider;
	bool mTextAppear;
	glm::vec3 mTextColor;

private:
	RTTR_ENABLE(Component)
};