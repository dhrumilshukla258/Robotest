/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/


#include "../pch.h"

#include "ResourceManager.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "../Shaders/Mesh.h"
#include "../Utilities/Reflection.h"
#include "../InitData.h"

#include <dirent.h>
#include <rapidjson/filereadstream.h>
using namespace rapidjson;

extern GraphicsManager* gpGraphicsManager;
extern SceneManager* gpSceneManager;
extern AudioManager* gpAudioManager;
extern Reflection* gpReflection;
extern float SCR_WIDTH;
extern float SCR_HEIGHT;
extern std::string glsl_version;

Document* ReadJSON(std::string, Document*);

ResourceManager::ResourceManager()
{
	setInitialParameters("Resources/Assets/init.json");
}

ResourceManager::~ResourceManager()
{
	for (auto doc : mPrefabs) {
		delete doc.second;
	}
	mTextures.clear();
	mMeshes.clear();
	mModel.clear();
}


void ResourceManager::setInitialParameters(std::string _filename)
{
	Document* pDoc = new Document;
	ReadJSON(_filename, pDoc);
	GenericObject<false, Value::ValueType> doc = pDoc->GetObject();
	directory::script = doc["Script_dir"].GetString();
	directory::prefab = doc["Prefab_dir"].GetString();
	directory::texture = doc["Texture_dir"].GetString();
	directory::object = doc["Object_dir"].GetString();
	directory::audio = doc["Audio_dir"].GetString();
	directory::level = doc["Level_dir"].GetString();
	directory::serialize = doc["Serialize_dir"].GetString();
	FlyEngine::General::start_scene = doc["Start_Scene"].GetString();
	
	auto loadArray = doc["Level_Sequence"].GetArray();
	for (SizeType i = 0; i < loadArray.Size(); i++) {
		FlyEngine::General::mLevelSequence.push_back(loadArray[i].GetString());
	}

	glsl_version = doc["glsl_version"].GetString();
	delete pDoc;

	ReadAllPrefabs();
}


void ResourceManager::ReadAllPrefabs()
{
	DIR *pDIR;
	struct dirent *entry;
	if (pDIR = opendir(directory::prefab.c_str())) {
		while (entry = readdir(pDIR)) {
			std::string prefab_name = entry->d_name;
			if (prefab_name.find(".json") != string::npos) {
				Document* pDoc = LoadPrefab(prefab_name);
				if(pDoc) mPrefabNames.push_back(prefab_name);
			}
		}
		closedir(pDIR);
	}
}


Document* ResourceManager::LoadPrefab(std::string _filename)
{
	Document* pDoc = mPrefabs[_filename];
	//Already Loaded?
	if (pDoc != nullptr)
		return pDoc;
	else {
		pDoc = new Document;
		std::string fullPath = directory::prefab + _filename;
		ReadJSON(fullPath, pDoc);
		mPrefabs[_filename] = pDoc;
		return pDoc;
	}
}

unsigned int ResourceManager::LoadTexture(std::string _filename)
{
	std::string fullPath = directory::texture + _filename;

	//Check Hash Map
	unsigned int textureId = mTextures[_filename];

	if (textureId) {
		return textureId;
	}

	//Load it
	textureId = gpGraphicsManager->LoadTextureCall(fullPath);

	//Load Successfull? Add to hash map
	if (textureId) {
		mTextures[_filename] = textureId;
	}

	//return to user
	return textureId;
}

unsigned int ResourceManager::LoadMesh(std::string _filename) 
{
	unsigned int VAO = mMeshes[_filename];

	if (VAO) {
		return VAO;
	}

	if (_filename == "Rekt") {
		VAO = gpGraphicsManager->CreateRekt();
	}
	else if (_filename == "Triangle") {
		VAO = gpGraphicsManager->CreateTriangle();
	}
	else if (_filename == "Circle") {
		VAO = gpGraphicsManager->CreateCircle();
	}
	else if (_filename == "Line") {
		VAO = gpGraphicsManager->CreateLine();
	}
	else if (_filename == "Rekt_Part") {
		VAO = gpGraphicsManager->CreateRekt_Particle();
	}
	
	//Load Successfull? Add to hash map
	if (VAO) {
		mMeshes[_filename] = VAO;
	}

	//return to user
	return VAO;
}

 std::vector<Mesh> ResourceManager::LoadModel(std::string _filename)
{
	std::string fullPath = directory::object + _filename;
	std::vector<Mesh> modelMeshes = mModel[_filename];

	//Check Hash Map
	if (!modelMeshes.empty()) {
		return modelMeshes;
	}

	//Load it
	modelMeshes = gpGraphicsManager->LoadModelCall(fullPath);

	if (!modelMeshes.empty()) {
		mModel[_filename] = modelMeshes;
	}

	return modelMeshes;
}
 
 /*void ResourceManager::LoadAudios()
 {
	 std::string currentScene = gpSceneManager->getCurrentScene();
	 size_t pos = currentScene.find(".json");
	 if (pos != string::npos) {
		 currentScene.replace(pos, currentScene.length(), "");
	 }

	 if ("" != currentScene) {
		 std::string audio_file = currentScene + "Audio.json";
		 std::string fullPath = "Resources/Assets/Levels/" + audio_file;

		 Document* pDoc = nullptr;
		 if ("" != audio_file)
			 pDoc = ReadJSON(fullPath, pDoc);

		 if (pDoc) {
			 GenericObject<false, Value::ValueType> AudioFile = pDoc->GetObject();
			 
			 if (AudioFile.HasMember("Load")) {
				 auto loadArray = AudioFile["Load"].GetArray();
				 for (SizeType i = 0; i < loadArray.Size(); i++)
				 {
					 std::string audio = loadArray[i].GetString();
					 gpAudioManager->Load(audio);
				 }

				 auto loadLoopArray = AudioFile["LoadLoop"].GetArray();
				 for (SizeType i = 0; i < loadLoopArray.Size(); i++)
				 {
					 std::string audio = loadLoopArray[i].GetString();
					 gpAudioManager->LoadLoop(audio);
				 }
			 }
		 }
	 }
 }
*/
//std::vector<Mesh> ResourceManager::LoadModel(std::string _filename, Stencil_Toon_Model *temp)
//{
//	std::string fullPath = directory::object + _filename;
//	std::vector<Mesh> modelMeshes = mModel[_filename];
//	
//	//Check Hash Map
//	if (!modelMeshes.empty()) {
//		return modelMeshes;
//	}
//
//	//Load it
//	modelMeshes = gpGraphicsManager->LoadModelCall(fullPath);
//
//	if (!modelMeshes.empty()) {
//		mModel[_filename] = modelMeshes;
//	}
//
//	return modelMeshes;
//}

Document* ResourceManager::ReadJSON(std::string _fullpath, Document* _doc)
{
	FILE *fp;
	fopen_s(&fp, _fullpath.c_str(), "r");
	//ASSERT("FlyEngine", fp, "File::ResourceManager Function::ReadJSON Details:: File {} not found", _fullpath);
	if (fp) {
		if (!_doc) {
			_doc = new Document;
		}
		char buffer[256] = { 0 };
		FileReadStream stream(fp, buffer, sizeof(buffer));
		_doc->ParseStream(stream);
		fclose(fp);
	}
	return _doc;
}