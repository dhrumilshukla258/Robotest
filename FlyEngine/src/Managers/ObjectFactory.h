/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#pragma once

#include "../Entity.h"
#include <vector>
using namespace rapidjson;

class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();

	//void LoadLevel(Document* _pDoc);
	//Entity* LoadObject(std::string _filename);
	Entity* CreateRuntimeObject(std::string);
	void DeleteObject(unsigned int);
	void Update();
public:
	std::vector<unsigned int> mDeletedObjs;
};