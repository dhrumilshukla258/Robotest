/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Author: Dhrumil Shukla, dhrumil.shukla 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#include "../pch.h"
#include "MapListStructure.h"

// Definitions
MapListStructure::MapListStructure()
{
}

MapListStructure::~MapListStructure()
{
	delete[] block_component;
	//mActiveVec.clear();
	mInactive.clear();
}

Component * MapListStructure::GetHead()
{
	return mActiveHead;
}

void MapListStructure::AttachToLL(Component* compo)
{
	if (mActiveHead == nullptr) {
		mActiveHead = compo;
	}
	else {
		mActiveHead->SetPrevious(compo);
		compo->SetPrevious(nullptr);
		compo->SetNext(mActiveHead);
		mActiveHead = compo;
	}
}

Component* MapListStructure::Add(unsigned int id)
{
	Component* compo = mInactive.front();
	AttachToLL(compo);
	mInactive.pop_front();
	return compo;
}

Component* MapListStructure::GetComponent(unsigned int id) {
	Component* c = GetHead();
	while ( nullptr != c) {
		if (id == c->mpOwner->id) {
			return c;
		}
		c = c->GetNext();
	}
	return nullptr;
}


void MapListStructure::DetachFromLL(Component* _comp)
{
	Component* next = _comp->GetNext();
	Component* pre = _comp->GetPrevious();
	if (_comp == mActiveHead) {
		if (nullptr != next) {
			next->SetPrevious(nullptr);
			mActiveHead = next;
		}
		else {
			mActiveHead = nullptr;
		}
	}
	else {
		if (nullptr != next) {
			pre->SetNext(next);
			next->SetPrevious(pre);
		}
		else { pre->SetNext(nullptr); }
	}

	_comp->SetNext(nullptr);
	_comp->SetPrevious(nullptr);
}

void MapListStructure::Delete(Component* _comp)
{
	DetachFromLL(_comp);
	mInactive.push_front(_comp);
}

// FOR EDITOR
void MapListStructure::AddToBuffer(Component* _comp)
{
	DetachFromLL(_comp);
	mEditorBuffer[_comp->mpOwner->id] = _comp;
}

void MapListStructure::RemoveFromBuffer(unsigned int _id)
{
	Component* pComp = mEditorBuffer[_id];
	AttachToLL(pComp);
	mEditorBuffer.erase(_id);
}

void MapListStructure::ClearFromBuffer(unsigned int _id)
{
	Component* pComp = mEditorBuffer[_id];
	mInactive.push_front(pComp);
	mEditorBuffer.erase(_id);
}