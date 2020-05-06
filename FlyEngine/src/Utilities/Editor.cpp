/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Fenil Shingala, fenil.shingala, 60003118
Author: Ivan Cazares, ivan.c, 60004418
- End Header --------------------------------------------------------*/
#include "../pch.h"
#include "Editor.h"

#include "../Managers/InputManager.h"
#include "../Managers/ObjectFactory.h"
#include "../Components/Transform.h"
#include "../Components/Body.h"
#include "../Components/Collider.h"
#include "../Components/Model.h"
#include "../Components/Stencil_Toon_Model.h"
#include "Reflection.h"
#include "RayCast.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/EntityManager.h"
#include "../Managers/SceneManager.h"
#include "../Managers/CameraManager.h"
#include "../Managers/SystemManager.h"
#include "../Systems/PlayerSystem.h"


#include "../Entity.h"
#include "ShapeStructure.h"

extern InputManager* gpInputManager;
extern ObjectFactory* gpObjectFactory;
extern Reflection* gpReflection;
extern ResourceManager* gpResourceManager;
extern ComponentManager* gpComponentManager;
extern EntityManager* gpEntityManager;
extern SceneManager* gpSceneManager;
extern GLFWwindow* gpWindow;
extern RayCast* gpRayCast;
extern CameraManager * gpCameraManager;
extern SystemManager * gpSystemManager;
extern float SCR_HEIGHT;
extern float SCR_WIDTH;


Editor::Editor() : mEditMode(false), mCurrentBinding(nullptr), pTr(nullptr), pBody(nullptr),
				mPrefabs(gpResourceManager->getPrefabList())
{

#ifndef _DEBUG
	debugCamera = false;
#endif // !_DEBUG

}

Editor::~Editor()
{
}

void Editor::check_mouse_click(double xpos, double ypos) {
	ypos = SCR_HEIGHT - ypos;

	Component* c = gpComponentManager->mBody->GetHead();
	Transform *pTr;
	while (nullptr != c) {
		pTr = static_cast<Transform*>(c);

		if ((xpos < pTr->mPos.x + pTr->mScale.x && xpos > pTr->mPos.x - pTr->mScale.x) ||
			(ypos < pTr->mPos.y + pTr->mScale.y && ypos > pTr->mPos.y - pTr->mScale.y))
		{
			mCurrentBinding = pTr->mpOwner;
			return;
		}

		c = c->GetNext();
	}
}

void Editor::ChangePtrAndBody() {
	pTr = mCurrentBinding->GetComponent<Transform>();
	pBody = mCurrentBinding->GetComponent<Body>();
	pCollider = mCurrentBinding->GetComponent<Collider>();
}

void property_viewer(Component* _component);

Component* selectedComponent = nullptr;

Entity* pSelectedEntity = nullptr;
Component* compSelect = nullptr;
void entity_viewer(Entity* temp) 
{
	if (!temp) return;
	Body* pB = temp->GetComponent<Body>();
	Transform* pT = temp->GetComponent<Transform>();
	Collider* pC = temp->GetComponent<Collider>();
	glm::vec3 prevScale = pT->mScale;
	float prevRotate = pT->mRotate;

	if (ImGui::BeginMenu(("Entity" + std::to_string(temp->id)).c_str()))
	{
		for (auto component_itr : temp->mComponents) {
			bool isSelected = false;
			bool wasSelected = false;

			if (compSelect) {
				if (compSelect == component_itr) { wasSelected = isSelected = true; }
			}

			ImGui::MenuItem(component_itr->mComponentName.c_str(), NULL, &isSelected);
			if (!wasSelected && isSelected) {
				compSelect = component_itr;
			}
		}
		ImGui::EndMenu();
	}

	if (pT->mScale != prevScale || pT->mRotate != prevRotate)
		pT->mDirtyFlag = true;
	if (pB) {
		if (pB->mPrevPos != glm::vec2(pB->pTr->mPos))
			pT->mDirtyFlag = true;
		pB->Init();
	}
	if (pC) {
		pC->Init();
	}
}

void property_viewer(Component* _component)
{
	rttr::type component = rttr::type::get_by_name(_component->mComponentName);
	for (auto prop : component.get_properties()) {
		
		if (prop.get_type() == rttr::type::get<int>()) {
			int temp = prop.get_value(_component).convert<int>();
			ImGui::InputInt((prop.get_name().to_string()).c_str(), &temp, 1, 0);
			prop.set_value(_component, temp);
		}
		else if (prop.get_type() == rttr::type::get<float>()) {
			float temp = prop.get_value(_component).convert<float>();
			ImGui::InputFloat((prop.get_name().to_string()).c_str(), &temp, 1.0f, 0, 3);
			prop.set_value(_component, temp);
		}
		else if (prop.get_type() == rttr::type::get<bool>()) {
			bool temp = prop.get_value(_component).convert<bool>();
			ImGui::Checkbox((prop.get_name().to_string()).c_str(), &temp);
			prop.set_value(_component, temp);
		}
		else if (prop.get_type() == rttr::type::get<std::string>()) {
			std::string temp = prop.get_value(_component).convert<std::string>();
			char x[100] = {""};
			const char* y = temp.c_str();
			strcpy_s(x, sizeof(x), y);
			
			ImGui::InputText((prop.get_name().to_string()).c_str(), x, 100);
			temp = x;
			prop.set_value(_component, temp);
		}
		else if (prop.get_type() == rttr::type::get<glm::vec3>()) {
			glm::vec3 temp = prop.get_value(_component).convert<glm::vec3>();
			ImGui::InputFloat((prop.get_name().to_string() + "X").c_str(), &temp.x, 1.0f, 0, 3);
			ImGui::InputFloat((prop.get_name().to_string() + "Y").c_str(), &temp.y, 1.0f, 0, 3);
			ImGui::InputFloat((prop.get_name().to_string() + "Z").c_str(), &temp.z, 1.0f, 0, 3);
			prop.set_value(_component, temp);
		}
		/*else if (prop.get_type() == rttr::type::get<glm::vec4>()) {
			glm::vec4 temp = prop.get_value(_component).convert<glm::vec4>();
			ImGui::InputFloat((prop.get_name().to_string() + "X").c_str(), &temp.x, 1.0f, 0, 3);
			ImGui::InputFloat((prop.get_name().to_string() + "Y").c_str(), &temp.y, 1.0f, 0, 3);
			ImGui::InputFloat((prop.get_name().to_string() + "Z").c_str(), &temp.z, 1.0f, 0, 3);
			ImGui::InputFloat((prop.get_name().to_string() + "W").c_str(), &temp.w, 1.0f, 0, 3);
			prop.set_value(_component, temp);
		}*/
		else if (prop.get_type() == rttr::type::get<glm::vec2>()) {
			glm::vec2 temp = prop.get_value(_component).convert<glm::vec2>();
			ImGui::InputFloat((prop.get_name().to_string() + "X").c_str(), &temp.x, 1.0f, 0, 3);
			ImGui::InputFloat((prop.get_name().to_string() + "Y").c_str(), &temp.y, 1.0f, 0, 3);
			prop.set_value(_component, temp);
		}
		else if (prop.get_type() == rttr::type::get<Entity*>()) {
			Entity* temp = prop.get_value(_component).convert<Entity*>();
			entity_viewer(temp);
		}
		else if (prop.get_type() == rttr::type::get<std::vector<Entity*>>()) {
			std::vector<Entity*> temp = prop.get_value(_component).convert<std::vector<Entity*>>();
			if (ImGui::BeginMenu(("EntityVec")))
			{
				for (auto entity : temp) {
					entity_viewer(entity);
				}
				ImGui::EndMenu();
			}
		}

	}
}

//glm::vec3 color_save;
std::string mCurrentBindingName;

void Editor::SetCurrentBinding(Entity* _pEntity)
{
#ifdef _DEBUG
	if (mCurrentBinding) {
		Model* pModel = mCurrentBinding->GetComponent<Model>();
		if (pModel)
			pModel->isSelected = false;
		else {
			Stencil_Toon_Model* pSTMd = mCurrentBinding->GetComponent<Stencil_Toon_Model>();
			if(pSTMd)
				pSTMd->isSelected = false;
		}
	}
#endif

	mCurrentBinding = _pEntity;
	rttr::enumeration type_enum = rttr::type::get_by_name("EntityType").get_enumeration();
	std::string type_name = type_enum.value_to_name(mCurrentBinding->mType).to_string();
	mCurrentBindingName = type_name + std::to_string(mCurrentBinding->id).c_str();
	ChangePtrAndBody();

#ifdef _DEBUG
	Model* pModel = mCurrentBinding->GetComponent<Model>();
	if (pModel) {
		mCurrentBinding->GetComponent<Model>()->isSelected = true;
	}
	else {
		Stencil_Toon_Model *pSTMD = mCurrentBinding->GetComponent<Stencil_Toon_Model>();
		if (pSTMD)
			pSTMD->isSelected = true;
	}
#endif
}


void Editor::SerializeOnSlot(unsigned int _slotNo)
{
	/*Model* pModel = nullptr;
	if (mCurrentBinding) {
		pModel = mCurrentBinding->GetComponent<Model>();
		if(pModel)
			pModel->mCol = color_save;
	}*/
	
	gpReflection->Serialize(_slotNo);
	
	/*if(pModel)
		pModel->mCol.y = 5.0f;*/
}

void Editor::Update()
{
	if (gpInputManager->isPressed(LCTRL)) {
		if (gpInputManager->isTriggered(P)) {//switch editor / play
			if (mEditMode) {
			#ifdef _DEBUG
				if (mCurrentBinding) {
					Model* pModel = mCurrentBinding->GetComponent<Model>();
					if(pModel) pModel->isSelected = false;
					mCurrentBinding = nullptr;
				}
			#endif
				gpEntityManager->ClearEditorBuffer();
			}
			mEditMode = !mEditMode;
		}
	}

	if (mEditMode) {
		if (gpInputManager->isPressed(LCTRL)) {
			if (gpInputManager->isPressed(LSHIFT)) {
				if (gpInputManager->isTriggered(F)) {// IF want to align a selected GameObject to the camera Position (X,Y,0)
					if (mCurrentBinding) {
						Transform *goPosition = mCurrentBinding->GetComponent<Transform>();
						if (goPosition) {
						goPosition->mPos = glm::vec3(gpCameraManager->mPosition.x , gpCameraManager->mPosition.y, 0);
						}
					}
				}
				if (gpInputManager->isTriggered(A)) {
				//	FocusOnPlayer();
				}
			}
			else {
				if (gpInputManager->isTriggered(A)) {
				//	FocusPlayerOnCamera();
				}
			}
			//////////////////////////////  LEFT CLICK TO SELECT ENTITY ///////////////////////////
			if (gpInputManager->isLeftClicked()) {
				Entity* e =  gpRayCast->CastThisShit();
				if (e) {
					if (mCurrentBinding) {
						SetCurrentBinding(e);
					}
					else {
						SetCurrentBinding(e);
					}
					selectedComponent = mCurrentBinding->GetComponent<Transform>();
				}
			}

			////////////////////////////// SAVE SCENE IN RESPECTIVE SLOT //////////////////////////////
			if (gpInputManager->isTriggered(ZERO))
				SerializeOnSlot(0);
			if (gpInputManager->isTriggered(ONE))
				SerializeOnSlot(1);
			if (gpInputManager->isTriggered(TWO))
				SerializeOnSlot(2);
			if (gpInputManager->isTriggered(THREE))
				SerializeOnSlot(3);
			if (gpInputManager->isTriggered(FOUR))
				SerializeOnSlot(4);
			if (gpInputManager->isTriggered(FIVE))
				SerializeOnSlot(5);
			if (gpInputManager->isTriggered(SIX))
				SerializeOnSlot(6);
			if (gpInputManager->isTriggered(SEVEN))
				SerializeOnSlot(7);
			if (gpInputManager->isTriggered(EIGHT))
				SerializeOnSlot(8);
			if (gpInputManager->isTriggered(NINE))
				SerializeOnSlot(9);

			////////////////////////////// DUPLICATE SELECTED GAME OBJECT IN SCENE //////////////////////////////

			if (gpInputManager->isTriggered(D)) {
				// if is pointing a GameObject. make a copy of it and all its components. to insert into the scenario
				if (mCurrentBinding) {
					//create a new game object
					Entity* pNewEntity = gpEntityManager->CreateEntity();
					pNewEntity->mType = mCurrentBinding->mType;
					//copy all the components from the current to nasew 
					for (auto comp : mCurrentBinding->mComponents) {
						MapListStructure* pMLS = gpReflection->getPoolObject(comp->mComponentName);
						Component* pComp = pMLS->Add(pNewEntity->id);
						pComp->mpOwner = pNewEntity;

						pComp->Clone(comp);
						pNewEntity->mComponents.push_back(pComp);
					}
					// make the new  go the current 
					
					/*Model* pCurrentModel = mCurrentBinding->GetComponent<Model>();
					if (pCurrentModel) {
						Model* pNewModel = pNewEntity->GetComponent<Model>();
						pNewModel->isSelected = false;
					}
					else {
						Stencil_Toon_Model* pNewSTMd = pNewEntity->GetComponent<Stencil_Toon_Model>();
						pNewSTMd->isSelected = false;
					}*/

					SetCurrentBinding(pNewEntity);//mCurrentBinding = pNewEntity;
					selectedComponent = mCurrentBinding->GetComponent<Transform>();
					mUndoStack.push(mCurrentBinding->id);
					mUndoOp.push(1);
				}
			}

			//////////////////////////////SNAP MOVEMENT OF SELECTED GAME OBJECT //////////////////////////////
			if (mCurrentBinding) {
				bool updateMovement = false;
				if (gpInputManager->isTriggered(LEFT)) {
					updateMovement = true;
					Transform *goPosition = mCurrentBinding->GetComponent<Transform>();
					if (goPosition)
						goPosition->mPos = glm::vec3(goPosition->mPos.x - movementOffset, goPosition->mPos.y, goPosition->mPos.z);;
				}
				if (gpInputManager->isTriggered(RIGHT)) {
					updateMovement = true;
					Transform *goPosition = mCurrentBinding->GetComponent<Transform>();
					if (goPosition)
						goPosition->mPos = glm::vec3(goPosition->mPos.x + movementOffset, goPosition->mPos.y, goPosition->mPos.z);;
				}
				if (gpInputManager->isTriggered(UP)) {
					updateMovement = true;
					Transform *goPosition = mCurrentBinding->GetComponent<Transform>();
					if (goPosition)
						goPosition->mPos = glm::vec3(goPosition->mPos.x, goPosition->mPos.y + movementOffset, goPosition->mPos.z);;
				}
				if (gpInputManager->isTriggered(DOWN)) {
					updateMovement = true;
					Transform *goPosition = mCurrentBinding->GetComponent<Transform>();
					if (goPosition)
						goPosition->mPos = glm::vec3(goPosition->mPos.x, goPosition->mPos.y - movementOffset, goPosition->mPos.z);
				}
				if (updateMovement) 
				{
					Oscillation *go = mCurrentBinding->GetComponent<Oscillation>();
					if (go)
						go->mReset = true;
				}
			}

			///////////////////////////////////////////    UNDO   //////////////////////////////////////////////
			if ( gpInputManager->isTriggered(Z))
			{
				if (!mUndoOp.empty()) {
					if (1 == mUndoOp.top()) {
						// created undo
						gpEntityManager->AddToEditorBuffer(mUndoStack.top());
					}
					if (0 == mUndoOp.top()) {
						// deleted undo
						gpEntityManager->RemoveFromEditorBuffer(mUndoStack.top());
					}
					// Id stack
					mRedoStack.push(mUndoStack.top());
					mUndoStack.pop();
					// Op stack
					mRedoOp.push(mUndoOp.top());
					mUndoOp.pop();
				}
			}

			///////////////////////////////////////////    REDO   //////////////////////////////////////////////
			if ( gpInputManager->isTriggered(Y))
			{
				if (!mRedoOp.empty()) {
					if (1 == mRedoOp.top()) {
						// created redo
						gpEntityManager->RemoveFromEditorBuffer(mRedoStack.top());
					}
					if (0 == mRedoOp.top()) {
						// deleted redo
						gpEntityManager->AddToEditorBuffer(mRedoStack.top());
					}
					// Id stack
					mRedoStack.pop();
					// Op stack
					mRedoOp.pop();
				}
			}
		}

		else if(gpInputManager->isPressed(LSHIFT)) {//loads a previous  recorded level in the slot
			
	////////////////////////////// LOAD SCENE FROM SLOT //////////////////////////////
				if (gpInputManager->isTriggered(ZERO)) {//load previous serialized saved file
					gpSceneManager->sameLevel = true;
					gpSceneManager->setNextScene(gpSceneManager->getLastSerializedScene(0));
					mCurrentBinding = nullptr;
				}
				if (gpInputManager->isTriggered(ONE)) {
					gpSceneManager->sameLevel = true;
					gpSceneManager->setNextScene(gpSceneManager->getLastSerializedScene(1));
					mCurrentBinding = nullptr;
				}
				if (gpInputManager->isTriggered(TWO)) {
					gpSceneManager->sameLevel = true;
					gpSceneManager->setNextScene(gpSceneManager->getLastSerializedScene(2));
					mCurrentBinding = nullptr;
				}
				if (gpInputManager->isTriggered(THREE)) {
					gpSceneManager->sameLevel = true;
					gpSceneManager->setNextScene(gpSceneManager->getLastSerializedScene(3));
					mCurrentBinding = nullptr;
				}
				if (gpInputManager->isTriggered(FOUR)) {
					gpSceneManager->sameLevel = true;
					gpSceneManager->setNextScene(gpSceneManager->getLastSerializedScene(4));
					mCurrentBinding = nullptr;
				}
				if (gpInputManager->isTriggered(FIVE)) {
					gpSceneManager->sameLevel = true;
					gpSceneManager->setNextScene(gpSceneManager->getLastSerializedScene(5));
					mCurrentBinding = nullptr;
				}
				if (gpInputManager->isTriggered(SIX)) {
					gpSceneManager->sameLevel = true;
					gpSceneManager->setNextScene(gpSceneManager->getLastSerializedScene(6));
					mCurrentBinding = nullptr;
				}
				if (gpInputManager->isTriggered(SEVEN)) {
					gpSceneManager->sameLevel = true;
					gpSceneManager->setNextScene(gpSceneManager->getLastSerializedScene(7));
					mCurrentBinding = nullptr;
				}
				if (gpInputManager->isTriggered(EIGHT)) {
					gpSceneManager->sameLevel = true;
					gpSceneManager->setNextScene(gpSceneManager->getLastSerializedScene(8));
					mCurrentBinding = nullptr;
				}
				if (gpInputManager->isTriggered(NINE)) {
					gpSceneManager->sameLevel = true;
					gpSceneManager->setNextScene(gpSceneManager->getLastSerializedScene(9));
					mCurrentBinding = nullptr;
				}

				if (gpInputManager->isTriggered(R)) {//Reload actual scene
					gpSceneManager->sameLevel = true;
					gpSceneManager->setNextScene(gpSceneManager->getCurrentScene());
					mCurrentBinding = nullptr;
				}
				if (gpInputManager->isTriggered(Y)) {//Switch camera mode
					debugCamera = !debugCamera;
					if (debugCamera) {
					}
					else {
						//FocusOnPlayer();
					}
				}

				//////////////////////////////SNAP MOVEMENT OF SELECTED GAME OBJECT //////////////////////////////
				if (mCurrentBinding) {
					if (gpInputManager->isTriggered(LEFT)) {
						Transform *go = mCurrentBinding->GetComponent<Transform>();
						if (go)
							go->mScale = glm::vec3(go->mScale.x - scaleOffset, go->mScale.y, go->mScale.z);;
					}
					if (gpInputManager->isTriggered(RIGHT)) {
						Transform *go = mCurrentBinding->GetComponent<Transform>();
						if (go)
							go->mScale = glm::vec3(go->mScale.x + scaleOffset, go->mScale.y, go->mScale.z);;
					}
					if (gpInputManager->isTriggered(UP)) {
						Transform *go = mCurrentBinding->GetComponent<Transform>();
						if (go)
							go->mScale = glm::vec3(go->mScale.x, go->mScale.y, go->mScale.z + scaleOffset);;
					}
					if (gpInputManager->isTriggered(DOWN)) {
						Transform *go = mCurrentBinding->GetComponent<Transform>();
						if (go)
							go->mScale = glm::vec3(go->mScale.x, go->mScale.y, go->mScale.z - scaleOffset);;
					}
				}
			}

		if (gpInputManager->isTriggered(DELETE1))
			DeleteActualGameObject();

		bool open = true;
		////////////////////////////// ALIVE OBJECT IMGUI //////////////////////////////
		//ImGui::SetNextWindowPos(ImVec2(0, 250));
		ImGui::Begin("Scene Objects");// , &open, ImVec2(166, 213), 0.9f, ImGuiWindowFlags_NoResize);
		//ImGui::SetWindowSize(ImVec2(166, 213));

		/*, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | 
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);*/

		for (auto& entity : gpEntityManager->mEntity) {
			if (entity.second) {

				rttr::enumeration type_enum = rttr::type::get_by_name("EntityType").get_enumeration();
				std::string type_name = type_enum.value_to_name(entity.second->mType).to_string();
				std::string str = type_name + std::to_string(entity.second->id).c_str();

				if (ImGui::Button(str.c_str())) {
					SetCurrentBinding(entity.second);
					selectedComponent = mCurrentBinding->GetComponent<Transform>();
				//	FocusOnSelected();
				}
			}
		}

		ImGui::End();
		///////////////////////////////////////////////////////////////////////////////


		///////////////////////////////// PREFABS IMGUI ///////////////////////////////
		//ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("Prefabs");// , &open, ImVec2(125, 247), 0.9f, ImGuiWindowFlags_NoResize);
		//ImGui::SetWindowSize(ImVec2(125, 247));

		/*if (ImGui::Button("Refresh Prefabs")) {
			mPrefabs = gpResourceManager->getPrefabList();
		}*/

		for (auto prefab : gpResourceManager->mPrefabNames) {
			std::string button_name = prefab.substr(0, prefab.length()-5);
			
			if (ImGui::Button(button_name.c_str())) {
				SetCurrentBinding(gpReflection->LoadObject(prefab));
				selectedComponent = mCurrentBinding->GetComponent<Transform>();
				Transform *goPosition = mCurrentBinding->GetComponent<Transform>();
				if (goPosition) {// initializes in the x and y camera position. ()
					glm::vec3 posInit = glm::vec3(gpCameraManager->mPosition.x, gpCameraManager->mPosition.y, 0);
					goPosition->mPos = posInit;
				}
				for (auto comp : mCurrentBinding->mComponents) {
					comp->Init();
				}

				//FocusOnSelected();
					
				mUndoStack.push(mCurrentBinding->id);
				mUndoOp.push(1);
			}
		}
		ImGui::End();
		///////////////////////////////////////////////////////////////////////////////


		//////////////////////////// CHARACTER CONTROL IMGUI //////////////////////////
		//ImGui::SetNextWindowPos(ImVec2(0, 467));
		ImGui::Begin("Inspector");//, &open, ImVec2(296, 360), 0.9f, ImGuiWindowFlags_NoResize);
		//ImGui::SetWindowSize(ImVec2(296, 360));

		glm::vec3 prevScale(0.0f);
		float prevRotate = 0.0f;
		glm::vec3 prevPos(0.0f);
		if (mCurrentBinding) {
			if (pTr) {
				glm::vec3 prevScale = pTr->mScale;
				float prevRotate = pTr->mRotate;
				glm::vec3 prevPos = pTr->mPos;
			}

			ImGui::Text(mCurrentBindingName.c_str());

			// COMPONENT LIST
			if (ImGui::BeginMenu("Components"))
			{
				for (auto component_itr : mCurrentBinding->mComponents) {
					bool isSelected = false;
					bool wasSelected = false;

					if (selectedComponent == component_itr) { wasSelected = isSelected = true; }

					ImGui::MenuItem(component_itr->mComponentName.c_str(), NULL, &isSelected);
					if (!wasSelected && isSelected) {
						selectedComponent = component_itr;
						compSelect = nullptr;
					}
				}
				ImGui::EndMenu();
			}
			if (selectedComponent) {
				property_viewer(selectedComponent);
			}
			if (compSelect)
				property_viewer(compSelect);

			if (pBody) {
				pBody->Init();
			}
			if (pCollider) {
				pCollider->Init();
			}
			if (pTr) {
				if (pTr->mScale != prevScale || pTr->mRotate != prevRotate || pTr->mPos != prevPos) {
					pTr->mDirtyFlag = true;
				}
			}
		}
		ImGui::End();
	}///////////////////////////////////       EDIT MODE       ////////////////////// /   

	 if (gpInputManager->isTriggered(F)) {
		FocusOnSelected();
	}
}

Entity* CheckCollision(glm::vec3 rayPos, Transform* pTran)
{
	Model* pModel = pTran->mpOwner->GetComponent<Model>();
	Collider* pCollider = pTran->mpOwner->GetComponent<Collider>();
	RawModel* pRawModel = pTran->mpOwner->GetComponent<RawModel>();

	glm::vec3 entityPos = pTran->mPos;
	glm::vec3 entityScale = pTran->mScale;

	if (pModel && !pCollider) {
		entityScale.y = pModel->y_max * pTran->mScale.z - pModel->y_min * pTran->mScale.z;
		entityScale.x = pModel->x_max * pTran->mScale.x - pModel->x_min * pTran->mScale.x;
		entityScale.z = pModel->z_max * pTran->mScale.y - pModel->z_min * pTran->mScale.y;
	}
	else if(pRawModel){
		entityScale.y = pRawModel->y_max * pTran->mScale.z - pRawModel->y_min * pTran->mScale.z;
		entityScale.x = pRawModel->x_max * pTran->mScale.x - pRawModel->x_min * pTran->mScale.x;
		entityScale.z = pRawModel->z_max * pTran->mScale.y - pRawModel->z_min * pTran->mScale.y;
	}

	if ( (entityPos.x + (entityScale.x / 2.0f) < rayPos.x || rayPos.x < entityPos.x - (entityScale.x / 2.0f)) ||
	  (entityPos.y + (entityScale.z / 2.0f) < rayPos.y || rayPos.y < entityPos.y - (entityScale.z / 2.0f))
		)
	{
		return nullptr;
	}

	glm::vec3 lookAt = gpCameraManager->mFront;
	float zConstraint = 0.0f;

	if (fabsf(lookAt.x) > fabsf(lookAt.y)) {
		if (fabsf(lookAt.x) > fabsf(lookAt.z)) {
			zConstraint = entityScale.x;
		}
		else {
			zConstraint = entityScale.y;
		}
	}
	else {
		if (fabsf(lookAt.y) > fabsf(lookAt.z)) {
			zConstraint = entityScale.z;
		}
		else {
			zConstraint = entityScale.y;
		}
	}

	if (fabsf(entityPos.z - rayPos.z) < zConstraint) {
		return pTran->mpOwner;
	}

	return nullptr;
}

Entity* CheckCollision(glm::vec3 rayPos, glm::vec3 Pos2, Shape* _shape)
{
	ShapeAABB* shape = static_cast<ShapeAABB*>(_shape);
	glm::vec2 r1extents = shape->GetHalfExtents();

	if( (Pos2.x + r1extents.x < rayPos.x || rayPos.x < Pos2.x - r1extents.x) || 
		(Pos2.y + r1extents.y < rayPos.y || rayPos.y < Pos2.y - r1extents.y) )
			
	{
		return nullptr;
	}
	
	glm::vec3 lookAt = gpCameraManager->mFront;
	float zConstraint = 0.0f;

	if (fabsf(lookAt.x) > fabsf(lookAt.y)) {
		if (fabsf(lookAt.x) > fabsf(lookAt.z)) {
			zConstraint = shape->mpOwnerCollider->mpTr->mScale.x;
		}
		else {
			zConstraint = shape->mpOwnerCollider->mpTr->mScale.y;
		}
	}
	else {
		if (fabsf(lookAt.y) > fabsf(lookAt.z)) {
			zConstraint = shape->mpOwnerCollider->mpTr->mScale.z;
		}
		else {
			zConstraint = shape->mpOwnerCollider->mpTr->mScale.y;
		}
	}

	if (fabsf(Pos2.z - rayPos.z) < zConstraint) {
		return shape->mpOwnerCollider->mpOwner;
	}
	
	return nullptr;
}

void Editor::DeleteActualGameObject()
{
	if (mCurrentBinding) {
		mUndoStack.push(mCurrentBinding->id);
		mUndoOp.push(0);

		gpEntityManager->AddToEditorBuffer(mCurrentBinding->id);
		mCurrentBinding = nullptr;
	}
}

void Editor::FocusOnSelected() {
	if (mCurrentBinding) {
		Transform *actualT = mCurrentBinding->GetComponent<Transform>();
		if (actualT)
		{
			glm::vec3 pos = actualT->mPos;
			gpCameraManager->mPosition = glm::vec3(pos.x, pos.y, 100.0f);
			gpCameraManager->LookFront();
		}
	}
}



void Editor::FocusOnPlayer() {

	//Entity* pPlayer =   gpSystemManager->mPlayerSystem->GetPlayerEntity();
	//if (pPlayer) {
	//	mCurrentBinding = pPlayer;
	//	FocusOnSelected();
	//}
}

void Editor::FocusPlayerOnCamera() {
	Entity* pPlayer = gpSystemManager->mPlayerSystem->GetPlayerEntity();
	if (pPlayer) {
		mCurrentBinding = pPlayer;
		Transform *goPosition = mCurrentBinding->GetComponent<Transform>();
		if (goPosition) {
			goPosition->mPos = glm::vec3(gpCameraManager->mPosition.x, gpCameraManager->mPosition.y, goPosition->mPos.z);
		}
	}
}


