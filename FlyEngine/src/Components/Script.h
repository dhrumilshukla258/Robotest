#pragma once

#include "../Component.h"

class Script : public Component {
public:
	Script();
	~Script();
	void Init();
	void SelfDelete();
	void Clone(Component *);

public:
	std::string mLuaFile;
	bool onEachFrame;
	float x;

protected:
	RTTR_ENABLE(Component)
};