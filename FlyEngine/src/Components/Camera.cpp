/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Camera.h"
#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<Camera>("Camera")
		 .property("MinBound", &Camera::mMinBound)
		 .property("MaxBound", &Camera::mMaxBound)
		 .property("keepFollow", &Camera::keepFollow);
}

Camera::Camera() : keepFollow(true)
{
	mComponentName = "Camera";
}


Camera::~Camera()
{
}

void Camera::SelfDelete() {
	gpComponentManager->mCamera->Delete(this);
}

void Camera::Clone(Component * rhs1) {
	Camera* rhs = static_cast<Camera*>(rhs1);


}