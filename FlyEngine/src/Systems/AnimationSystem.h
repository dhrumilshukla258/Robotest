#pragma once
#include "../Shaders/Shader.h"
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "../Shaders/Mesh.h"
using namespace std;
class AnimationSystem
{
public:
	AnimationSystem();
	~AnimationSystem();
	void Update();

	void BoneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms);
	int findPosition(float p_animation_time, const aiNodeAnim* p_node_anim);

	int findRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
	int findScaling(float p_animation_time, const aiNodeAnim* p_node_anim);

	aiVector3D CalcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
	aiQuaternion CalcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
	aiVector3D CalcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim);

	const aiNodeAnim * findNodeAnim(const aiAnimation * p_animation, const std::string p_node_name);

	aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend);

	

	void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform);



public:

	const aiScene* scene;
	aiMatrix4x4 m_global_inverse_transform;

	std::map<std::string, int> m_bone_mapping; // maps a bone name and their index
	int m_num_bones = 0;
	std::vector<BoneMatrix> m_bone_matrices;
	std::vector<aiMatrix4x4> transforms;

	GLuint m_bone_location[100];
	float ticks_per_second = 0.0f;


	int mCurrent_animation;
	int mPrevious_animation;
	float mStart_Time=0.0f;
	float mStop_Time=1.0f;
	float mAnimation_Speed=1.0f;
	bool mAnimation_Repeat;
	bool mAnimation_Stop ;
	list<int>  mAnimation_Stack;
	float mAnimation_Time;
	int temp;
	bool temp_bool;

	int enemy_player_anim;
	

	
};