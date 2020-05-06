/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/


#include "../pch.h"
#include "CameraManager.h"
#include "SceneManager.h"
#include "SystemManager.h"
#include "../Systems/PlayerSystem.h"
#include "../Components/Transform.h"
#include "../Utilities/Editor.h"


extern SystemManager* gpSystemManager;
extern SceneManager* gpSceneManager;
extern float SCR_WIDTH;
extern float SCR_HEIGHT;
extern Editor * gpEditor;	
extern bool isPaused;
extern bool gameStarted;

const glm::vec3 MAIN_MENU_POSITION = glm::vec3(40.0f, 9.0f, 150.0f);
const glm::vec3 DP_LOGO_POSITION = glm::vec3(250.0f, 5.0f, 150.0f);

// global variables 
float finalPos = 0.0f, originalPos = 0.0f, originalYPos = 0.0f;
bool isTweening = false;
bool isTweeningVertical = false;

void CameraManager::InitializeCamera() {
	if (!isInitialized) {
		isGoingRight = true;
		isInitialized = true;
		mPosition.x = lookAtPoint->mPos.x + CAMERA_MOVE_OFFSET;
		mPosition.y = lookAtPoint->mPos.y;
	}
}

CameraManager::CameraManager() :
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)), mPosition(glm::vec3(0.0f, 0.0f, 50.0f)),
	mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mWorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mRight(glm::vec3(1.0f, 0.0f, 0.0f)), mMovementSpeed(SPEED), mPitch(PITCH),
	isInitialized(false), isGoingRight(true), keepFollowing(true),
	mYaw(YAW), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM), cameraXOffset(0.0f),
	view(reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16))),
	proj(reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16)))
{
	UpdateCameraVectors();
	
	cameraXOffset = -CAMERA_MOVE_OFFSET;

	tweenRegions = tweeny::from(0.0f).to(1.0f).during(200.0f);
	tweenUpDown = tweeny::from(0.0f).to(1.0f).during(2000.0f);
	tweenFollow = tweeny::from(0.0f).to(1.0f).during(2000.0f);

	tweenUpDown.via(0, tweeny::easing::quadraticOut);
	tweenRegions.via(0, tweeny::easing::quadraticInOut);
	tweenFollow.via(0, tweeny::easing::quadraticOut);

	mPosition = DP_LOGO_POSITION;
}

void CameraManager::ResetCamera() {
	isTweening = false;
	isTweeningVertical = false;
	tweenUpDown.seek(0);
	tweenRegions.seek(0);
	tweenFollow.seek(0);
	if (gpSceneManager->getCurrentScene() == "LevelBoss.json" && keepFollowing == false) {
		mPosition = cBounds.maxBounds;
	} else {
		isInitialized = false;
		InitializeCamera();
	}
}

void CameraManager::SetBounds(glm::vec3 min, glm::vec3 max) {
	cBounds.minBounds.x = min.x;
	cBounds.minBounds.y = min.y;
	cBounds.minBounds.z = min.z;
	cBounds.maxBounds.x = max.x;
	cBounds.maxBounds.y = max.y;
	cBounds.maxBounds.z = max.z;

	CAMERA_MOVE_OFFSET = 0.15f * cBounds.minBounds.z;
	PLAYER_MOVE_OFFSET = 0.40f * cBounds.minBounds.z;
	Y_MOVE_OFFSET = 0.1f * cBounds.minBounds.z;

	keepFollowing = false;
}

CameraManager::~CameraManager()
{
	_aligned_free(view);
	_aligned_free(proj);
}

void CameraManager::SetLookAtPoint(Body* pTr) {
	lookAtPoint = pTr;
}

void CameraManager::SetTweenToPosition(glm::vec3 pos) {
	tweenRegions.seek(0);
	tweenFromPos = mPosition;
	tweenToPos = pos;
}

void CameraManager::SnapToPosition(glm::vec3 pos) {
	mPosition = pos;
	tweenFromPos = pos;
	tweenToPos = pos;
}

void CameraManager::LookFront() {
	mYaw = -90;
	mPitch = 0;
	UpdateCameraVectors();
}

void CameraManager::Update(InputManager* input) {

#ifdef _DEBUG
	ProcessInput(input);
	{
		//bool open = true;
		//ImGui::SetNextWindowPos(ImVec2(1196, 465));
		ImGui::Begin("Camera");//, &open, ImVec2(241, 56), 0.9f, ImGuiWindowFlags_NoResize);
		//ImGui::SetWindowSize(ImVec2(241, 56));
		ImGui::InputFloat3("Camera Position", &mPosition.x);
		ImGui::InputFloat("Pitch", &mPitch);
		ImGui::InputFloat("Yaw", &mYaw);
		
		/*ImGui::Text("playerVelocity = %f %f", playerBody->mVel.x, playerBody->mVel.y);
		ImGui::Text("playerPosition = %f %f", playerBody->mPos.x, playerBody->mPos.y);
		ImGui::Text("mTotalForce in X dir = %f", playerBody->mTotalForce.x);
		ImGui::Checkbox("isGrounded", &gpSystemManager->mPlayerSystem->jumpState.isGrounded);
		ImGui::Checkbox("jumpingUp", &gpSystemManager->mPlayerSystem->jumpState.jumpingUp);
		ImGui::Checkbox("jumpingDown", &gpSystemManager->mPlayerSystem->jumpState.jumpingDown);*/
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
#endif
	
	if (gpEditor->debugCamera)// if  camera in debug mode has control , dont manage camaera
	{
		isInitialized = false;
	}
	else {
		if (!gameStarted || isPaused) {
			// camera setting for Main Menu and Pause Menu
			//mPosition = MAIN_MENU_POSITION;
			mPosition = tweenFromPos + tweenRegions.step(0.016f) * (tweenToPos - tweenFromPos);
			mPosition.z = 150.0f;
		}
		else if (gpSceneManager->getCurrentScene() == "LevelBoss.json" && keepFollowing == false) {
			mPosition = cBounds.maxBounds;
		}
		else {
			keepFollowing = true;

			if (lookAtPoint != nullptr) {
				InitializeCamera();

				if (isTweeningVertical) {
					mPosition.y = originalYPos + tweenUpDown.step(0.008f) * (lookAtPoint->mPos.y - originalYPos);
					mPosition = glm::clamp(mPosition, cBounds.minBounds, cBounds.maxBounds);
					if (tweenUpDown.progress() > 0.99f) {
						tweenUpDown.seek(0);
						isTweeningVertical = false;
					}
				}
				if (isTweening) {
					mPosition.x = originalPos + tweenFollow.step(0.016f) * (finalPos - originalPos);
					mPosition = glm::clamp(mPosition, cBounds.minBounds, cBounds.maxBounds);
					if (isGoingRight)
						finalPos = lookAtPoint->mPos.x + CAMERA_MOVE_OFFSET;
					else
						finalPos = lookAtPoint->mPos.x - CAMERA_MOVE_OFFSET;
					if (tweenFollow.progress() > 0.99f) {
						isTweening = false;
						tweenFollow.seek(0);
					}
					return;
				}

				if (isGoingRight) {
					if (lookAtPoint->mPos.x > mPosition.x - CAMERA_MOVE_OFFSET) {
						mPosition.x = lookAtPoint->mPos.x + CAMERA_MOVE_OFFSET;
					}
					else if (lookAtPoint->mPos.x < mPosition.x - PLAYER_MOVE_OFFSET) {
						isTweening = true;
						isGoingRight = false;
						originalPos = mPosition.x;
						finalPos = mPosition.x - (PLAYER_MOVE_OFFSET + CAMERA_MOVE_OFFSET);
					}
				}
				else {

					if (lookAtPoint->mPos.x < mPosition.x + CAMERA_MOVE_OFFSET) {
						mPosition.x = lookAtPoint->mPos.x - CAMERA_MOVE_OFFSET;
					}
					else if (lookAtPoint->mPos.x > mPosition.x + PLAYER_MOVE_OFFSET) {
						originalPos = mPosition.x;
						finalPos = mPosition.x + PLAYER_MOVE_OFFSET + CAMERA_MOVE_OFFSET;
						isGoingRight = true;
						isTweening = true;
					}
				}

				// for y position of camera
				if (abs(mPosition.y - lookAtPoint->mPos.y) > Y_MOVE_OFFSET && !isTweeningVertical) {
					isTweeningVertical = true;
					originalYPos = mPosition.y;
				}

				// clamping to level bounds
				mPosition = glm::clamp(mPosition, cBounds.minBounds, cBounds.maxBounds);

				//// to make camera play well with bounds
				//if (mPosition.x == cBounds.minBounds.x) {
				//	isGoingRight = true;
				//}
				//else if (mPosition.x == cBounds.maxBounds.x) {
				//	isGoingRight = false;
				//}
			}
		}
	}
}

//===== for use in edit mode
void  CameraManager::ProcessInput(InputManager* input) {
	glm::vec3 boost;

	if (input->isRightClicked()) {

		boost = input->isPressed(LSHIFT) ? glm::vec3(5,5,5) : glm::vec3(1, 1, 1);
		// for advancing faster with shift

		if (input->isPressed(W))
			mPosition += mFront * boost * mMovementSpeed;
		if (input->isPressed(S))
			mPosition -= mFront * boost * mMovementSpeed;
		if (input->isPressed(A))
			mPosition -= mRight * boost * mMovementSpeed;
		if (input->isPressed(D))
			mPosition += mRight * boost * mMovementSpeed;
		if (input->isPressed(E))
			mPosition += mUp * boost * mMovementSpeed;
		if (input->isPressed(Q))
			mPosition -= mUp * boost * mMovementSpeed;

	}
}

void CameraManager::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
{
	xoffset *= mMouseSensitivity;
	yoffset *= mMouseSensitivity;

	mYaw += xoffset;
	mPitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (mPitch > 89.0f)
			mPitch = 89.0f;
		if (mPitch < -89.0f)
			mPitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	UpdateCameraVectors();
}

void CameraManager::ProcessMouseScroll(float yoffset)
{
	if (mZoom >= 1.0f && mZoom <= 45.0f)
		mZoom -= yoffset;
	if (mZoom <= 1.0f)
		mZoom = 1.0f;
	if (mZoom >= 45.0f)
		mZoom = 45.0f;
}

//======= edit mode code ends
glm::mat4& CameraManager::GetViewMatrix() {
	
	*view = glm::lookAt(mPosition, mPosition + mFront, mUp);
	return *view;
}
glm::mat4& CameraManager::GetProjectionMatrix() {
	*proj = glm::perspective(glm::radians(mZoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 20000.0f);
	return *proj;
}

// Up and Right vectors might never change
// use this when needed to recalculate
// 2 sq.roots
void CameraManager::UpdateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::cross(mRight, mFront);
}