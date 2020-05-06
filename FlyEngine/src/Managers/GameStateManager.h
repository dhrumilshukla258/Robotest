/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#pragma once

class GameStateManager {
public:
	static void Init();
	static void SafeDelete();
	static void SetUpGl();
	static void StartEngine();
	static void Update();
};