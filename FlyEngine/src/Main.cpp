/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Author: Dhrumil Shukla, dhrumil.shukla 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#include "pch.h"
//#include <vld.h>
#include "Managers/GameStateManager.h"

#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main(int argc, char* args[])
{
	GameStateManager::Init();
	GameStateManager::StartEngine();
	GameStateManager::SafeDelete();

	return 0;
}