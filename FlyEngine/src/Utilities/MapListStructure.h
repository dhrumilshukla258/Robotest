/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Author: Dhrumil Shukla, dhrumil.shukla 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#pragma once

#include <forward_list>
#include <unordered_map>
#include "../Component.h"
#include "../Managers/EntityManager.h"

extern EntityManager* gpEntityManager;


class MapListStructure
{
private:
	Component* block_component;
	std::forward_list<Component*> mInactive;
	Component* mActiveHead;
	std::unordered_map<unsigned int, Component*> mEditorBuffer;

public:
	MapListStructure();
	~MapListStructure();

	Component* GetHead();
	Component* Add(unsigned int id);
	void Delete(Component*);

	// FOR EDITOR
	void AddToBuffer(Component*);
	void RemoveFromBuffer(unsigned int);
	void ClearFromBuffer(unsigned int);

	Component* GetComponent(unsigned int id);

	template <typename T, unsigned int N>
	void generatePool()
	{
		T* a = new T[N];
		block_component = a;
		for (int i = 1; i < N-1; ++i) {
			a[i].SetpMLS(this);
			mInactive.push_front(&a[i]);
		}
	}

private:
	void DetachFromLL(Component*);
	void AttachToLL(Component*);
};