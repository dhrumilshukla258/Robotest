/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Author: Dhrumil Shukla, dhrumil.shukla 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#pragma once

#include "pch.h"

using namespace rapidjson;

class Entity;
class MapListStructure;
class Component
{
public:
	Component();
	virtual ~Component() = 0;
	// virtual void Serialize(GenericObject<false, Value::ValueType>) = 0;
	virtual void Init() = 0;
	virtual void SelfReset();
	virtual void SelfDelete() = 0;
	virtual void Clone(Component*) {}
	virtual void DeleteChildReference(unsigned int) {}

	// FOR EDITOR
	void SelfBuffer();
	void SelfBufferRemove();
	void SelfBufferClear();

	Component* GetPrevious();
	Component* GetNext();

	// Caution on using below function
	void SetPrevious(Component* _comp);
	void SetNext(Component* _comp);
	void SetpMLS(MapListStructure* _pml);

public:
	Entity* mpOwner = nullptr;
	std::string mComponentName;
private:
	MapListStructure* pMLS;
	Component* mPrevious = nullptr;
	Component* mNext = nullptr;
protected:
	RTTR_ENABLE()
};