/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#pragma once
#include "../Entity.h"
#include <forward_list>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();
	Entity* CreateEntity();
	void DeleteEntity(unsigned int);

	// FOR EDITOR
	void AddToEditorBuffer(unsigned int);
	void RemoveFromEditorBuffer(unsigned int);
	void ClearEditorBuffer();

public:
	unsigned int id = 0;
	Entity * block;
	std::forward_list<Entity *> mInactive;
	std::unordered_map<unsigned int, Entity *> mEntity;
	std::unordered_map<unsigned int, Entity *> mEditorBufferEntities;
};