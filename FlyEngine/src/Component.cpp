/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Author: Dhrumil Shukla, dhrumil.shukla 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#include "pch.h"

#include "Component.h"
#include "Managers/ComponentManager.h"
#include "Entity.h"

extern ComponentManager* gpComponentManager;

Component::Component() : mpOwner(nullptr)
{
}

Component::~Component()
{
}

//void Component::SelfDelete()
//{
//	pMLS->Delete(this);
//}
void Component::SelfReset()
{

}


// FOR EDITOR
void Component::SelfBuffer()
{
	pMLS->AddToBuffer(this);
}

void Component::SelfBufferRemove()
{
	pMLS->RemoveFromBuffer(mpOwner->id);
}

void Component::SelfBufferClear()
{
	pMLS->ClearFromBuffer(mpOwner->id);
}
// ----------

Component * Component::GetPrevious()
{
	return mPrevious;
}

Component * Component::GetNext()
{
	return mNext;
}

void Component::SetPrevious(Component * _comp)
{
	mPrevious = _comp;
}

void Component::SetNext(Component * _comp)
{
	mNext = _comp;
}

void Component::SetpMLS(MapListStructure * _pml)
{
	pMLS = _pml;
}