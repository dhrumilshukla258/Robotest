/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#pragma once

class RenderSystem;
class PlayerSystem;
class PhysicsSystem;
class BehaviourSystem;
class ControlSystem;
class TimerSystem;
class HealthSystem;
class RenderCalculation;
class ParticleSystem;
class OscillationSystem;
class ScriptingSystem;
class LauncherSystem;
class DoorSystem;
class HomingSystem;
class AIBehaviour;
class LightSystem;
class AnimationSystem;
class BossSystem;
class TextRenderer;

class SystemManager
{
public:
	SystemManager();
	~SystemManager();
	void Init();
	void Update(float);

public:
	RenderSystem* mRenderSystem;
	PlayerSystem* mPlayerSystem;
	PhysicsSystem* mPhysicsSystem;
	BehaviourSystem* mBehaviourSystem;
	ControlSystem* mControlSystem;
	TimerSystem* mTimerSystem;
	HealthSystem* mHealthSystem;
	RenderCalculation* mRenderCalculation;
	ParticleSystem* mParticleSystem;
	OscillationSystem* mOscillationSystem;
	DoorSystem* mDoorSystem;
	ScriptingSystem* mScriptingSystem;
	LauncherSystem* mMissileLauncherSystem;
	HomingSystem* mHomingSystem;
	AIBehaviour* mAIBehaviour;
	LightSystem* mLightSystem;
	AnimationSystem* mAnimationSystem;
	BossSystem* mBossSystem;
	TextRenderer* mTextRenderer;
};