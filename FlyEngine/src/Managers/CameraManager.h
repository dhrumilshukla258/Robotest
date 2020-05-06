/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#pragma once

class InputManager;
class Body;
#include <tweeny/tweeny.h>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


struct CameraBounds {
	glm::vec3 minBounds = glm::vec3(0.0f);
	glm::vec3 maxBounds = glm::vec3(0.0f);
};

class CameraManager
{
public:
	float PLAYER_MOVE_OFFSET = 30.0f;
	float CAMERA_MOVE_OFFSET = 10.0f;
	float Y_MOVE_OFFSET = 10.0f;

	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;
	// Euler Angles
	float mYaw;
	float mPitch;
	// Camera options
	float mMovementSpeed;
	float mMouseSensitivity;
	float mZoom;

	bool keepFollowing;
public:
	CameraManager();
	~CameraManager();

	glm::mat4& GetViewMatrix();
	glm::mat4& GetProjectionMatrix();
	void ProcessInput(InputManager* input);
	void ProcessMouseMovement(float, float, bool);
	void ProcessMouseScroll(float);
	void Update(InputManager* input);
	void SetBounds(glm::vec3 min, glm::vec3 max);
	void SetLookAtPoint(Body* pTr);
	void LookFront();
	void InitializeCamera();
	void SetTweenToPosition(glm::vec3 pos);
	void SnapToPosition(glm::vec3 pos);
	void ResetCamera();
private:
	tweeny::tween<float> tweenRegions;
	tweeny::tween<float> tweenUpDown;
	tweeny::tween<float> tweenFollow;
	
	float newYPos, originalYPos;
	float cameraXOffset;

	bool isGoingRight;
	bool isInitialized;

	CameraBounds cBounds;

	// use in menu tweens
	glm::vec3 tweenToPos;
	glm::vec3 tweenFromPos;

	Body* lookAtPoint;
	glm::mat4* view;
	glm::mat4* proj;
	void UpdateCameraVectors();
};

