#pragma once

#include "../Component.h"
#include "tweeny/tweeny.h"

class Entity;

class EnemyState {
public:
	EnemyState() : PatrolingState(false), ChasingState(false) {}
	~EnemyState() {}
public:
	bool PatrolingState;
	bool ChasingState;
};

enum EnemDir {
	ENEML = 0,
	ENEMR
};

class EnemyAI : public Component
{
public:
	EnemyAI();
	~EnemyAI();

	void Init();
	void SelfDelete();
	void DeleteChildReference(unsigned int);

	inline void SetChasing(bool _arg) { if (mState) mState->ChasingState = _arg; }
	inline void SetPatroling(bool _arg) { if (mState) mState->PatrolingState = _arg; }
	inline bool GetChasingState() { if (mState) return mState->ChasingState;  return false; }
	inline bool GetPatrolingState() { if (mState) return mState->PatrolingState;  return false; }

	void ResolveWayPoint(Entity*);
	float FlipEnemy(EnemDir);

public:
	// BOSS
	Entity* mBossOwner = nullptr;

	// DRONE
	float mShootGap;
	float mShootCurr;
	float mShootSpeed;
	bool mDroneActive;
	Entity* mDroneLOS;

	// GUARD
	std::string mEnemyType;
	Entity* mCurrentWayPoint;
	EnemDir mDir;
	float mGuardSpeed;
	Entity* mLOS_Box; // Line of sight box
	std::vector<Entity*> wpVec; // waypoint vector

private:
	unsigned int mCurrentWPIndex;
	EnemyState* mState;
	tweeny::tween<float> tweenL;
	tweeny::tween<float> tweenR;
protected:
	RTTR_ENABLE(Component)
};