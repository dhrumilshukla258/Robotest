/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#include "../pch.h"

#include "RayCast.h"
#include "ShapeStructure.h"

#include "../Managers/InputManager.h"
#include "../Managers/CameraManager.h"
#include "../Managers/ComponentManager.h"

extern float SCR_HEIGHT;
extern float SCR_WIDTH;
extern InputManager* gpInputManager;
extern CameraManager * gpCameraManager;
extern ComponentManager* gpComponentManager;


RayCast::RayCast()
{
}

RayCast::~RayCast()
{
}

Entity* RayCast::CastThisShit()
{
	glm::vec2 cursorCoord = InputManager::getCursorCoord();

	// window to NDC
	float x = (2.0f * cursorCoord.x) / SCR_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * cursorCoord.y) / SCR_HEIGHT;

	// NDC to camera space
	glm::vec4 ray_clip = glm::vec4(glm::vec2(x, y), -1.0f, 1.0f);
	glm::vec4 ray_eye = glm::inverse(gpCameraManager->GetProjectionMatrix()) * ray_clip;

	// camera to world space
	ray_eye = glm::vec4(glm::vec2(ray_eye), -1.0, 0.0);
	glm::vec3 direction = glm::vec3(glm::inverse(gpCameraManager->GetViewMatrix()) * ray_eye);

	direction = glm::normalize(direction);

	// shoot a ray
	glm::vec3 rayPosition = gpCameraManager->mPosition;
	float step = 0.05f;						// MAGIC NUMBER

	int n = 0;
	bool collided = false;
	while ((n++) < 3000 && !collided) {		// MAGIC NUMBER
		Transform* pTransform = static_cast<Transform*>(gpComponentManager->mTransform->GetHead());

		while (pTransform && !collided) {
			if (pTransform->mpOwner->mType == SKY_BOX) {
				pTransform = static_cast<Transform*>(pTransform->GetNext());
				continue;
			}

			Collider* pCollider = static_cast<Collider*>(pTransform->mpOwner->GetComponent<Collider>());

			// collision check
			//while (pCollider && !collided) {
			Entity* pCollidedEntity = nullptr;
			if (pCollider) {
				pCollidedEntity = CheckCollision(glm::vec3(rayPosition), pTransform->mPos, pCollider->mpShape);
				if (pCollidedEntity)
					return pCollidedEntity;
			}
			pTransform = static_cast<Transform*>(pTransform->GetNext());
		}
		rayPosition = gpCameraManager->mPosition + direction * step;
		step += 0.1f;
	}
	
	return nullptr;
}

Entity* RayCast::CheckCollision(glm::vec3 rayPos, Transform* pTran)
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
	else if (pRawModel) {
		entityScale.y = pRawModel->y_max * pTran->mScale.z - pRawModel->y_min * pTran->mScale.z;
		entityScale.x = pRawModel->x_max * pTran->mScale.x - pRawModel->x_min * pTran->mScale.x;
		entityScale.z = pRawModel->z_max * pTran->mScale.y - pRawModel->z_min * pTran->mScale.y;
	}

	if ((entityPos.x + (entityScale.x / 2.0f) < rayPos.x || rayPos.x < entityPos.x - (entityScale.x / 2.0f)) ||
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

Entity* RayCast::CheckCollision(glm::vec3 rayPos, glm::vec3 Pos2, Shape* _shape)
{
	ShapeAABB* shape = static_cast<ShapeAABB*>(_shape);
	glm::vec2 r1extents = shape->GetHalfExtents();

	if ((Pos2.x + r1extents.x < rayPos.x || rayPos.x < Pos2.x - r1extents.x) ||
		(Pos2.y + r1extents.y < rayPos.y || rayPos.y < Pos2.y - r1extents.y))

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