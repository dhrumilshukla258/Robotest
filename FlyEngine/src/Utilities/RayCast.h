/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#pragma once

class Entity;
class Transform;
class Shape;
class RayCast
{
public:
	RayCast();
	~RayCast();
	Entity* CastThisShit();
	Entity* CheckCollision(glm::vec3, Transform*);
	Entity* CheckCollision(glm::vec3 rayPos, glm::vec3 Pos2, Shape* _shape);
};

