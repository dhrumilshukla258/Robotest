/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#pragma once
class Collider;
enum ShapeType
{
	CIRCLE,
	AABB,
	SHAPESNUM
};

class Shape
{
public:

	Shape(ShapeType type)
	{
		mType = type;
		mpOwnerCollider = nullptr;
	}
	virtual ~Shape() {}
	virtual glm::vec2 GetHalfExtents() = 0;
	virtual bool TestPoint(float PointX, float PointY) = 0;

	Collider* mpOwnerCollider;
	//void Clone(Component *) {};


public:
	ShapeType mType;
};

class ShapeCircle : public Shape
{
public:
	ShapeCircle() : Shape(CIRCLE) { mRadius = 0.0f; }
	
	void SetValue(float radius)
	{
		mRadius = radius;
	}
	~ShapeCircle()
	{
	}
	bool TestPoint(float PointX, float PointY)
	{
		return false;
	}
	glm::vec2 GetHalfExtents() {
		glm::vec2 extents;
		extents = glm::vec2(mRadius, mRadius);
		return extents;
	}

	float mRadius;
};

class ShapeAABB : public Shape
{
public:
	ShapeAABB() : Shape(AABB) { 
		mTop = 0.0f;
		mBottom = 0.0f;
		mLeft = 0.0f;
		mRight = 0.0f;
	}
	void SetValue(float top, float bottom, float left, float right)
	{
		mTop = top;
		mBottom = bottom;
		mLeft = left;
		mRight = right;
	}
	~ShapeAABB()
	{
	}
	bool TestPoint(float PointX, float PointY)
	{
		return false;
	}
	glm::vec2 GetHalfExtents() {
		return glm::vec2((mRight-mLeft)/2, (mTop - mBottom)/2);
	}

	float mTop, mBottom, mLeft, mRight;
};