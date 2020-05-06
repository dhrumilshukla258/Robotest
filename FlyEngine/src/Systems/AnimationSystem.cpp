/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "AnimationSystem.h"
#include "../Managers/ComponentManager.h"
#include "../Managers/GraphicsManager.h"

#include "../Components/Animation.h"

#include "../Managers/FrameRateController.h"
#include "../Managers/CameraManager.h"
#include "../Managers/GraphicsManager.h"
#include <random>
#include "../Components/World.h"

extern ComponentManager* gpComponentManager;
extern FrameRateController* gpFrameRateController;
extern CameraManager* gpCameraManager;
extern GraphicsManager* gpGraphicsManager;


///////////////////
AnimationSystem::AnimationSystem()
{
	mAnimation_Stop = false;
}

AnimationSystem::~AnimationSystem()
{
}

void AnimationSystem::Update()
{
	Animation *pAnim = nullptr;
	Stencil_Toon_Model* pSTMd;
	Model* pMd;



	World* pWorld;
	Component* cb = gpComponentManager->mWorld->GetHead();

	pWorld = static_cast<World*>(cb);
	
	Component* c = gpComponentManager->mAnimation->GetHead();
	while (nullptr != c)
	{
		pAnim = static_cast<Animation*>(c);
		pMd = pAnim->mpOwner->GetComponent<Model>();
		pSTMd = pAnim->mpOwner->GetComponent<Stencil_Toon_Model>();

		mAnimation_Repeat = pAnim->mAnimation_Repeat;
		mAnimation_Stop = pAnim->mAnimation_Stop;
  		mAnimation_Stack = pAnim->mAnimation_Stack;
	
			
		mCurrent_animation = pAnim->mCurrent_animation;
		mPrevious_animation = pAnim->mPrevious_animation;
			
		mAnimation_Time = pAnim->mAnimation_Time;;

		if (pAnim->mpOwner->mType == PLAYER)
		{
 			if (pMd->ticks_per_second == 0.0f)
			{
				pMd->ticks_per_second = gpGraphicsManager->ticks_per_second;
				pMd->scene = gpGraphicsManager->pScene;
				pMd->m_global_inverse_transform = gpGraphicsManager->m_global_inverse_transform;
				pMd->m_num_bones = gpGraphicsManager->m_num_bones;
				pMd->m_bone_matrices = gpGraphicsManager->m_bone_matrices;
				pMd->ticks_per_second = gpGraphicsManager->ticks_per_second;
				pMd->transforms = gpGraphicsManager->transforms;

				pMd->m_bone_mapping = gpGraphicsManager->m_bone_mapping;
				pMd->animation = true;
				mAnimation_Repeat = true;
			}
			mStart_Time = (float)mCurrent_animation;
			mStop_Time = mStart_Time + 1;
			enemy_player_anim = 0;
		}
		else
		{
			mStart_Time = (float)mCurrent_animation;
			mStop_Time = mStart_Time + 1;
			enemy_player_anim = 1;
		}
		
		if (temp_bool)
		{
			temp = 0;
			mAnimation_Stack.push_back(temp);
			
			temp_bool = false;
		}
		if (pSTMd != nullptr)
		{
			scene = pSTMd->scene;
			m_global_inverse_transform = pSTMd->m_global_inverse_transform;
			m_bone_mapping = pSTMd->m_bone_mapping; // maps a bone name and their index
			m_num_bones = pSTMd->m_num_bones;
			m_bone_matrices = pSTMd->m_bone_matrices;
			ticks_per_second = pSTMd->ticks_per_second;
					
			BoneTransform(gpFrameRateController->GetFrameTime() *30.0f, pSTMd->transforms);
		}
		else if (pMd != nullptr)
		{
			
			scene = pMd->scene;
			   m_global_inverse_transform = pMd->m_global_inverse_transform;

			m_bone_mapping = pMd->m_bone_mapping; // maps a bone name and their index
			m_num_bones = pMd->m_num_bones;
			m_bone_matrices = pMd->m_bone_matrices;


			
			ticks_per_second = pMd->ticks_per_second;
					
			BoneTransform(gpFrameRateController->GetFrameTime()*30.0f, pMd->transforms);
		}
			
				
 		pAnim->mAnimation_Repeat = mAnimation_Repeat;
		pAnim->mAnimation_Stop=mAnimation_Stop ;
		pAnim->mCurrent_animation = mCurrent_animation ;
		pAnim->mPrevious_animation = mPrevious_animation;
		pAnim->mAnimation_Time=mAnimation_Time ;
		pAnim->mAnimation_Stack= mAnimation_Stack;
		///////////////////
		
		c = c->GetNext();
	}

}



void AnimationSystem::BoneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms)
{
	aiMatrix4x4 identity_matrix; 
	double time_in_ticks = time_in_sec * ticks_per_second;

	if (mAnimation_Repeat)
	{
		if (mAnimation_Stack.size() == 0)
		{
			//mStart_Time = mPrevious_animation;
			
			//mAnimation_Time = 24.0f*mStart_Time;
		}
		else
		{
			mStart_Time = (float)(mAnimation_Stack.front());
			mStop_Time = mStart_Time + 1;
			mAnimation_Stack.pop_front();
			mCurrent_animation = (int)mStart_Time;
		}

		mAnimation_Time = 24.0f*mStart_Time;
		mStop_Time = mStart_Time + 1;
		mAnimation_Repeat = false;
	}
	


	
	float start_time = mAnimation_Time;
	
	if (scene)
	{

		float stop_time;
		stop_time = ((float)(scene->mAnimations[0]->mDuration) / END_ANIMATION) * mStop_Time;
		/*if (enemy_player_anim == 0)
		{

			 stop_time = ((float)(scene->mAnimations[0]->mDuration) / END_ANIMATION) * mStop_Time;

		}
		else
		{
			
			 stop_time = ((float)(scene->mAnimations[0]->mDuration) / END_ENEMY_ANIMATION) * 3;
		}*/
				

		if (mAnimation_Time > stop_time*0.99f)
		{
			
			mPrevious_animation =mCurrent_animation;
			if (mAnimation_Stack.size() != 0)
			{
				mAnimation_Repeat = true;
			}
			else
			{
				
				
			}

		}
		else
		{
			mAnimation_Time += 0.016f*50.0f;
			//mAnimation_Time += (float)time_in_sec;
		}
		
		float animation_time;
		animation_time = fmod(start_time, stop_time);

		readNodeHierarchy(animation_time, scene->mRootNode, identity_matrix);





		transforms.resize(m_num_bones);

		for (int i = 0; i < m_num_bones; i++)
		{
			transforms[i] = m_bone_matrices[i].final_world_transform;
		}



	}
	
	
}
int AnimationSystem::findPosition(float p_animation_time, const aiNodeAnim * p_node_anim)
{

	for (uint i = 0; i < p_node_anim->mNumPositionKeys - 1; i++)
	{
		if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

int AnimationSystem::findRotation(float p_animation_time, const aiNodeAnim * p_node_anim)
{

	for (uint i = 0; i < p_node_anim->mNumRotationKeys - 1; i++)
	{
		if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}
int AnimationSystem::findScaling(float p_animation_time, const aiNodeAnim * p_node_anim)
{

	for (uint i = 0; i < p_node_anim->mNumScalingKeys - 1; i++)
	{
		if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}
aiVector3D AnimationSystem::CalcInterpolatedPosition(float p_animation_time, const aiNodeAnim * p_node_anim)
{
	if (p_node_anim->mNumPositionKeys == 1)
	{
		return p_node_anim->mPositionKeys[0].mValue;
	}

	uint position_index = findPosition(p_animation_time, p_node_anim);
	uint next_position_index = position_index + 1;
	assert(next_position_index < p_node_anim->mNumPositionKeys);

	float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - p_node_anim->mPositionKeys[position_index].mTime);

	float factor = (p_animation_time - (float)p_node_anim->mPositionKeys[position_index].mTime) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
	aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}
aiQuaternion AnimationSystem::CalcInterpolatedRotation(float p_animation_time, const aiNodeAnim * p_node_anim)
{
	if (p_node_anim->mNumRotationKeys == 1)
	{
		return p_node_anim->mRotationKeys[0].mValue;
	}

	uint rotation_index = findRotation(p_animation_time, p_node_anim);
	uint next_rotation_index = rotation_index + 1;
	assert(next_rotation_index < p_node_anim->mNumRotationKeys);

	float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);

	float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

	//cout << "p_node_anim->mRotationKeys[rotation_index].mTime: " << p_node_anim->mRotationKeys[rotation_index].mTime << endl;
	//cout << "p_node_anim->mRotationKeys[next_rotaion_index].mTime: " << p_node_anim->mRotationKeys[next_rotation_index].mTime << endl;
	//cout << "delta_time: " << delta_time << endl;
	//cout << "animation_time: " << p_animation_time << endl;
	//cout << "animation_time - mRotationKeys[rotation_index].mTime: " << (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) << endl;
	//cout << "factor: " << factor << endl << endl << endl;

	assert(factor >= 0.0f && factor <= 1.0f);
	aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
	aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

	return nlerp(start_quat, end_quat, factor);
}

aiVector3D AnimationSystem::CalcInterpolatedScaling(float p_animation_time, const aiNodeAnim * p_node_anim)
{
	if (p_node_anim->mNumScalingKeys == 1)
	{
		return p_node_anim->mScalingKeys[0].mValue;
	}

	uint scaling_index = findScaling(p_animation_time, p_node_anim);
	uint next_scaling_index = scaling_index + 1;
	assert(next_scaling_index < p_node_anim->mNumScalingKeys);

	float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);

	float  factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
	aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

const aiNodeAnim * AnimationSystem::findNodeAnim(const aiAnimation * p_animation, const std::string p_node_name)
{
	// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
	// numChannels == numBones
	for (uint i = 0; i < p_animation->mNumChannels; i++)
	{
		const aiNodeAnim* node_anim = p_animation->mChannels[i];
		if (std::string(node_anim->mNodeName.data) == p_node_name)
		{
			return node_anim;
		}
	}

	return nullptr;
}

aiQuaternion AnimationSystem::nlerp(aiQuaternion a, aiQuaternion b, float blend)
{
	//cout << a.w + a.x + a.y + a.z << endl;
	a.Normalize();
	b.Normalize();

	aiQuaternion result;
	float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	float one_minus_blend = 1.0f - blend;

	if (dot_product < 0.0f)
	{
		result.x = a.x * one_minus_blend + blend * -b.x;
		result.y = a.y * one_minus_blend + blend * -b.y;
		result.z = a.z * one_minus_blend + blend * -b.z;
		result.w = a.w * one_minus_blend + blend * -b.w;
	}
	else
	{
		result.x = a.x * one_minus_blend + blend * b.x;
		result.y = a.y * one_minus_blend + blend * b.y;
		result.z = a.z * one_minus_blend + blend * b.z;
		result.w = a.w * one_minus_blend + blend * b.w;
	}

	return result.Normalize();
}

void AnimationSystem::readNodeHierarchy(float p_animation_time, const aiNode * p_node, const aiMatrix4x4 parent_transform)
{
	string node_name(p_node->mName.data);


	const aiAnimation* animation = scene->mAnimations[0];
	aiMatrix4x4 node_transform = p_node->mTransformation;

	const aiNodeAnim* node_anim = findNodeAnim(animation, node_name);

	if (node_anim)
	{

		//scaling
		//aiVector3D scaling_vector = node_anim->mScalingKeys[2].mValue;
		aiVector3D scaling_vector = CalcInterpolatedScaling(p_animation_time, node_anim);
		aiMatrix4x4 scaling_matr;
		aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

		//rotation
		//aiQuaternion rotate_quat = node_anim->mRotationKeys[2].mValue;
		aiQuaternion rotate_quat = CalcInterpolatedRotation(p_animation_time, node_anim);
		aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

		//translation
		//aiVector3D translate_vector = node_anim->mPositionKeys[2].mValue;
		aiVector3D translate_vector = CalcInterpolatedPosition(p_animation_time, node_anim);
		aiMatrix4x4 translate_matr;
		aiMatrix4x4::Translation(translate_vector, translate_matr);

		/*if (string(node_anim->mNodeName.data) == string("Head"))
		{
			aiQuaternion rotate_head = aiQuaternion(rotate_head_xz.w, rotate_head_xz.x, rotate_head_xz.y, rotate_head_xz.z);

			node_transform = translate_matr * (rotate_matr * aiMatrix4x4(rotate_head.GetMatrix())) * scaling_matr;
		}
		else
		{
			node_transform = translate_matr * rotate_matr * scaling_matr;
		}*/

		node_transform = translate_matr * (rotate_matr)* scaling_matr;

	}

	aiMatrix4x4 global_transform = parent_transform * node_transform;



 	if (m_bone_mapping.find(node_name) != m_bone_mapping.end()) // true if node_name exist in bone_mapping
	{
		int bone_index = m_bone_mapping[node_name];
		m_bone_matrices[bone_index].final_world_transform = m_global_inverse_transform * global_transform * m_bone_matrices[bone_index].offset_matrix;
	}

	for (unsigned int i = 0; i < p_node->mNumChildren; i++)
	{
		readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);
	}
}


