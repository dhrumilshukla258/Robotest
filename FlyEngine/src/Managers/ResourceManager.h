/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#pragma once
#include "../Components/Stencil_Toon_Model.h"
using namespace rapidjson;
class Mesh;
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	//Document* LoadLevel(std::string _filename);
	Document* LoadPrefab(std::string _filename);
	unsigned int LoadTexture(std::string _filename);
	unsigned int LoadMesh(std::string _filename);
	
	std::vector<Mesh> LoadModel(std::string _filename);

	//void LoadAudios();

	Document* ReadJSON(std::string, Document*);
	void ReadAllPrefabs();
	std::unordered_map<std::string, Document*>& getPrefabList() { return mPrefabs; }
	void setInitialParameters(std::string _filename);

public:
	std::vector<std::string> mPrefabNames;	// editor purpose

private:
	std::unordered_map<std::string, Document*> mPrefabs;
	std::unordered_map<std::string, unsigned int> mTextures;
	std::unordered_map<std::string, unsigned int> mMeshes; //Mesh 2D
	std::unordered_map<std::string, std::vector<Mesh> > mModel; //Mesh 3D
};


