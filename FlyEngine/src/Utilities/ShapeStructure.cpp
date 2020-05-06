/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Author: Dhrumil Shukla, dhrumil.shukla 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#include "../pch.h"
#include "ShapeStructure.h"
#include "../Managers/ComponentManager.h"

extern ComponentManager* gpComponentManager;

ShapeStructure::ShapeStructure()
{
}

ShapeStructure::~ShapeStructure()
{
	delete[] block;
	mActive.clear();
	mInactive.clear();
}

Shape * ShapeStructure::Add(unsigned int id)
{
	mActive[id] = mInactive.front();
	mInactive.pop_front();
	return mActive[id];
}

void ShapeStructure::Delete(unsigned int id)
{
	if (mActive.find(id) != mActive.end()) {
		mInactive.push_front(mActive[id]);
		mActive.erase(id);
	}
}
