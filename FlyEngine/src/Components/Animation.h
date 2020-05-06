#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Shaders/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Shaders/Mesh.h"
#include "../Shaders/Shader.h"
#include "../Component.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "glm\gtc\quaternion.hpp"

#include <string>
using namespace std;

enum Animation_Type {
	IDLE_ANIMATION,
	JUMP_ANIMATION,
	FORWARD_ANIMATION,
	DEATH_ANIMATION,
	RESPAWN_ANIMATION,
	END_ANIMATION
};

enum Enemy_Animation_Type {
	IDLE_ENEMY_ANIMATION,
	HURT_ANIMATION,
	BACK_ANIMATION,
	FRONT_ANIMATION,
	LEFT_ANIMATION,
	LEFT_BACK_ANIMATION,
	RIGHT_ANIMATION,
	RIGHT_BACK_ANIMATION,
	END_ENEMY_ANIMATION
};

class Animation : public Component
{
public:
	Animation();
	~Animation();
	void Init();
	void SelfDelete();
	void Clone(Component *);

public:
	
	int mCurrent_animation;
	int mPrevious_animation;
	float mStart_Time;
	float mStop_Time;
	float mAnimation_Speed;
	bool mAnimation_Repeat;
	bool mAnimation_Stop;
	list<int>  mAnimation_Stack;
	float mAnimation_Time;

private:
	RTTR_ENABLE(Component)
};