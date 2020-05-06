/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "FrameRateController.h"

FrameRateController::FrameRateController(unsigned int _maxFrameRate)
{
	mTickEnd = mTickStart = mFrameTime = 0.0;

	if (0.0 != _maxFrameRate) {
		mNeededTicksPerFrame = 1.0 / _maxFrameRate;
	}
	else
		mNeededTicksPerFrame = 0.0;
}


FrameRateController::~FrameRateController()
{
}

void FrameRateController::FrameStart() {
	mTickStart = glfwGetTime();
	//printf("mTickStart : %lf\n", mTickStart);
}

void FrameRateController::FrameEnd() {
	mTickEnd = glfwGetTime();
	//printf("mTickEnd : %lf\n", mTickEnd);

	while (mTickEnd - mTickStart < mNeededTicksPerFrame) {
		mTickEnd = glfwGetTime();
	}

	mFrameTime = mTickEnd - mTickStart;
}

double FrameRateController::GetFrameTime() {
	return mFrameTime;
}