/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Author: Dhrumil Shukla, dhrumil.shukla 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#include "InitData.h"

std::string FlyEngine::Dir::script;
std::string FlyEngine::Dir::prefab;
std::string FlyEngine::Dir::texture;
std::string FlyEngine::Dir::object;
std::string FlyEngine::Dir::audio;
std::string FlyEngine::Dir::level;
std::string FlyEngine::Dir::serialize;

std::string FlyEngine::General::start_scene;
std::vector<std::string> FlyEngine::General::mLevelSequence;
sol::state FlyEngine::General::gLuaState;