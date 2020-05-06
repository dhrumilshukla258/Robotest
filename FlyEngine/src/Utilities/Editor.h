/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Fenil Shingala, fenil.shingala, 60003118
Author: Ivan Cazares, ivan.c, 60004418
- End Header --------------------------------------------------------*/
#pragma once

class Entity;
class Transform;
class Body;
class Collider;
using namespace rapidjson;

class Editor
{
public:
	Editor();
	~Editor();

	void Update();

private:
	void check_mouse_click(double xpos, double ypos);
	void ChangePtrAndBody();
	void DeleteActualGameObject();
	void FocusOnSelected();
	void FocusOnPlayer();
	void FocusPlayerOnCamera();
	void SetCurrentBinding(Entity*);
	void SerializeOnSlot(unsigned int _slotNo);

public:
	bool mEditMode;
	bool debugCamera = true;

private:
	Entity* mCurrentBinding;
	Transform* pTr;
	Body* pBody;
	Collider* pCollider;
	float movementOffset = 5.0f;
	float scaleOffset = 1.0f;
	
	std::unordered_map<std::string, Document*> mPrefabs;
	std::stack<unsigned int> mUndoStack;
	std::stack<unsigned int> mRedoStack;

	std::stack<unsigned int> mUndoOp;
	std::stack<unsigned int> mRedoOp;
};