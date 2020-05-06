/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Author: Dhrumil Shukla, dhrumil.shukla 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#include <string>
#include "SOL/sol.hpp"

#define lua FlyEngine::General::gLuaState
#define directory FlyEngine::Dir

namespace FlyEngine {
	class Dir {
	public:
		static std::string script;
		static std::string prefab;
		static std::string texture;
		static std::string object;
		static std::string audio;
		static std::string level;
		static std::string serialize;
	};

	class General {
	public:
		static std::string start_scene;
		static std::vector<std::string> mLevelSequence;
		static sol::state gLuaState;
	};
};