/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#pragma once
class FrameRateController
{
public:
	FrameRateController(unsigned int maxFrameRate);
	~FrameRateController();
	void FrameStart();
	void FrameEnd();
	double GetFrameTime();
private:
	double mTickStart;
	double mTickEnd;
	double mFrameTime;
	double mNeededTicksPerFrame;
};