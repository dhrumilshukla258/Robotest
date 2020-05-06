/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "SystemManager.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/PlayerSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Systems/ControlSystem.h"
#include "../Systems/TimerSystem.h"
#include "../Systems/HealthSystem.h"
#include "../Systems/RenderCalculation.h"
#include "../Systems/OscillationSystem.h"
#include "../Systems/ScriptingSystem.h"
#include "../Systems/LauncherSystem.h"
#include "../Systems/DoorSystem.h"
#include "../Systems/HomingSystem.h"
#include "../Systems/AIBehaviour.h"
#include "../Systems/LightSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/TextRenderer.h"
#include "../Systems/BossSystem.h"

#include "../Utilities/Editor.h"

float accumulator = 0.0f;
float maxPossible_dt = 0.016f; // 60Hz
int physicsUpdates = 0;

extern bool gEditMode;
extern bool isPaused;
#include "../Systems/ParticleSystem.h"

SystemManager::SystemManager()
{
	mScriptingSystem = new ScriptingSystem();
	mTimerSystem = new TimerSystem();
	mPhysicsSystem = new PhysicsSystem();
	mControlSystem = new ControlSystem();
	mRenderCalculation = new RenderCalculation();
	mParticleSystem = new ParticleSystem();
	mOscillationSystem = new OscillationSystem();
	mDoorSystem = new DoorSystem();
	mAIBehaviour = new AIBehaviour();
	mBossSystem = new BossSystem();
}

void SystemManager::Init()
{
	mRenderSystem = new RenderSystem();
	mPlayerSystem = new PlayerSystem();
	mHealthSystem = new HealthSystem();
	mMissileLauncherSystem = new LauncherSystem();
	mHomingSystem = new HomingSystem();
	mLightSystem = new LightSystem();
	mAnimationSystem = new AnimationSystem();
	mTextRenderer = new TextRenderer();
}

SystemManager::~SystemManager()
{
	delete mRenderSystem;
	delete mPlayerSystem;
	delete mPhysicsSystem;
	delete mControlSystem;
	delete mTimerSystem;
	delete mHealthSystem;
	delete mRenderCalculation;
	delete mParticleSystem;
	delete mOscillationSystem;
	delete mDoorSystem;
	delete mScriptingSystem;
	delete mMissileLauncherSystem;
	delete mHomingSystem;
	delete mAIBehaviour;
	delete mLightSystem;
	delete mAnimationSystem;
	delete mTextRenderer;
	delete mBossSystem;
}

void SystemManager::Update(float _frameTime)
{
	bool moveInEdit = false;// For testing particle modifications . normaly  this is false
	// physics update should be first because it may affect other updates
	if ( moveInEdit ||!gEditMode) {
		//LOG_INFO("ConsoleLogger", _frameTime);
		//== fixed physics update

		if (!isPaused) {
			physicsUpdates = 0;
			accumulator += _frameTime;
			while (accumulator > maxPossible_dt) {
				++physicsUpdates;
				mPhysicsSystem->Update(maxPossible_dt);
				accumulator -= maxPossible_dt;
			}
			mPhysicsSystem->ResetForces();
			mPhysicsSystem->InterpolateState(accumulator / maxPossible_dt);
			mPlayerSystem->Update(_frameTime);
			mMissileLauncherSystem->Update(_frameTime);
			mHealthSystem->Update();
			mDoorSystem->Update();
			mHomingSystem->Update(_frameTime);
			mTimerSystem->Update(_frameTime);
			mOscillationSystem->Update();
			mParticleSystem->Update();
			mScriptingSystem->Update(_frameTime);
			mAIBehaviour->Update(_frameTime);
			mBossSystem->Update(_frameTime);


			mAnimationSystem->Update();
		}
		mControlSystem->Update();
	}


	// this should always be last
	mLightSystem->Update();
	mRenderCalculation->Update();
#ifdef _DEBUG
	//bool open = true;
	//ImGui::SetNextWindowPos(ImVec2(1096, 0));
	ImGui::Begin("Frames!");// , &open, ImVec2(344, 105), 0.9f, ImGuiWindowFlags_NoResize); // Create a window called "Hello, world!" and append into it.
	//ImGui::SetWindowSize(ImVec2(344, 105));

	ImGui::Text("accumulator = %f", accumulator);
	ImGui::Text("physicsFrames = %d", physicsUpdates);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Debug Draw", &mRenderSystem->debug_Select);

	ImGui::End();

	//ImGui::SetNextWindowPos(ImVec2(1240, 104));
	ImGui::Begin("Physics");// , &open, ImVec2(198, 360), 0.9f, ImGuiWindowFlags_NoResize);
	//ImGui::SetWindowSize(ImVec2(198, 360));
	ImGui::SliderFloat("gravity", &mPhysicsSystem->gravity.y, 300.0f, -300.0f);
	ImGui::End();
#endif // _DEBUG
	mRenderSystem->Update();
}