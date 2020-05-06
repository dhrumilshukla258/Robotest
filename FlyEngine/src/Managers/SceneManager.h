/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#pragma once

using namespace rapidjson;
class Entity;
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Update();
	inline void setNextScene(std::string _scene) { next_scene = _scene; }
	inline std::string getCurrentScene() { return current_scene; }
	inline std::string getNextScene() { return next_scene; }
	void LoadNextLevel();
	void LoadLevel(std::string _filename);
	void ClearLevel();
	std::string getLastSerializedScene(unsigned int i);
public:
	std::vector<Entity*> mMenuEntity;
	bool sameLevel = false;
private:
	std::string current_scene = "";
	std::string next_scene = "";
	bool LoopPassed = false;
	std::unordered_map<std::string, Document*> mLevelPrefabs;
};