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
#include "../Components/Shape.h"

class ShapeStructure
{
public:
	Shape* block;
	std::forward_list<Shape*> mInactive;
	std::unordered_map<unsigned int, Shape*> mActive;
public:
	ShapeStructure();
	~ShapeStructure();

	Shape* Add(unsigned int id);
	void Delete(unsigned int id);

	template <typename T,unsigned int N>
	void generatePool()
	{
		T * a = new T[N];
		ASSERT("FlyEngine", a, "Memory not Allocated Properly in ShapeStructure file");
		block = a;
		for (int i = 0; i < N; ++i) {
			mInactive.push_front(&a[i]);
		}
	}
};

