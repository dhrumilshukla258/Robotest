/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "RenderSystem.h"

#include "../Managers/ComponentManager.h"
#include "../Managers/CameraManager.h"
#include "../Managers/GraphicsManager.h"
#include "../Components/Transform.h"
#include "../Components/ParticleGenerator.h"
#include "../Managers/FrameRateController.h"
#include "../Components/World.h"
#include "../Managers/SystemManager.h"
#include "../Systems/TextRenderer.h"

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

extern ComponentManager* gpComponentManager;
extern GraphicsManager* gpGraphicsManager;
extern CameraManager* gpCameraManager;
extern FrameRateController* gpFrameRateController;
extern GLFWwindow* gpWindow;
extern SystemManager* gpSystemManager;
extern float SCR_WIDTH;
extern float SCR_HEIGHT;

#define model_location 5
#define view_location 6
#define projection_location 7
#define shadow_Matrix 8

using namespace std;
RenderSystem::RenderSystem()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT);
	

	
	glDepthFunc(GL_LESS);
	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
	debug_Select = false;
	VAO = gpGraphicsManager->CreateRekt();
	gpGraphicsManager->Frame_Bubber_Set();
	VAO_HDR = gpGraphicsManager->Create_Frame_Buffer_Pointer();
	VAO_Quad = gpGraphicsManager->CreateQuad();
	//Solved the alignment issue by this stuff
	
	projection_light = reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16));
	lightView = reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16));
	shadow_matrix = reinterpret_cast<glm::mat4 *> (_aligned_malloc(sizeof(glm::mat4), 16));
}

void RenderSystem::Update()
{
	//*projection3D = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 5000.0f);
	*projection_light = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 5000.0f);
	//*projection_light = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 5000.0f);
	//*projection_light = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 2000.0f);
	//*view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	//*lightView = glm::lookAt(cameraPos, (cameraPos+ cameraFront), cameraUp);
//	ImGui::Begin("light");
//	
//	ImGui::InputFloat("X", &cameraPos.x, 0.0f, 0.0f);
//	ImGui::InputFloat("Y", &cameraPos.y, 0.0f, 0.0f);
//	ImGui::InputFloat("Z", &cameraPos.z, 0.0f, 0.0f);
//
//	ImGui::InputFloat("LX", &cameraFront.x, 0.0f, 0.0f);
//	ImGui::InputFloat("LY", &cameraFront.y, 0.0f, 0.0f);
//	ImGui::InputFloat("LZ", &cameraFront.z, 0.0f, 0.0f);
//	//*lightView = glm::lookAt(glm::vec3(x, y, z), glm::vec3(lx,ly, lz), glm::vec3(0.0f, 1.0f, 0.0f));
//	//*lightView = glm::lookAt(cameraPos, (cameraPos + cameraFront), cameraUp);
//	//*shadow_matrix = (*projection_light) * (*lightView);
//ImGui::End();
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_STENCIL_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//	glStencilMask(0x00);

	//Update2D();
	
	glViewport(0, 0, (GLsizei)SCR_WIDTH, (GLsizei)SCR_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, VAO_HDR);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glBindTexture(GL_TEXTURE_2D, gpGraphicsManager->colorBuffer);

	//glActiveTexture(GL_TEXTURE13);
	//glBindTexture(GL_TEXTURE_2D, gpGraphicsManager->colorBuffer);
	//GLint location = glGetUniformLocation(gpGraphicsManager->shader_HDR.ID, "hdrBuffer");
	//glUniform1i(location, 13);


	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	//Use_Shader3D_Shadow(gpGraphicsManager->shader3D_Shadow);
	//Update3D_Model(gpGraphicsManager->shader3D_Shadow);
	//Update3D_Background_Model(gpGraphicsManager->shader3D_Shadow);
	//Update3D_Stencil_Toon_Model(gpGraphicsManager->shader3D_Shadow);
	//End_Use_Shader3D_Shadow();

	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Use_Shader3D(gpGraphicsManager->shader3D);
	Update3D_Model(gpGraphicsManager->shader3D);
	
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//glStencilMask(0xFF);
	

	//Use_Shader3D_Stencil_Toon(gpGraphicsManager->shader3D);
	//Update3D_Stencil_Toon_Model(gpGraphicsManager->shader3D);
	
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilMask(0x00);
//	glDisable(GL_DEPTH_TEST);

	//Use_Stencil_Shader(gpGraphicsManager->shader_Stencil);
	//Update3D_Stencil_Toon_Model_Stencil(gpGraphicsManager->shader_Stencil);
	
	//glStencilMask(0xFF);
	//glEnable(GL_DEPTH_TEST);

	////Use_Shader3D_Stencil_Toon(gpGraphicsManager->shader3D);
	////Update3D_Stencil_Toon_Model(gpGraphicsManager->shader3D);

	//glAlphaFunc(GL_GEQUAL, 0.1f);
	//glEnable(GL_ALPHA_TEST);
	//glDisable(GL_STENCIL_TEST);
	
	if (debug_Select == 1)
	{
		Use_Shader3D_Debug(gpGraphicsManager->shader3D_Debug);
		Update3D_Debug(gpGraphicsManager->shader3D_Debug);
	}

	//	TEXT RENDERING
	gpSystemManager->mTextRenderer->Update();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	


	///////////////////////////
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 20;
	gpGraphicsManager->shader_Blur.use();

	//gpGraphicsManager->shader_Blur.setInt("image", 15);

	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gpGraphicsManager->pingpongFBO[horizontal]);
		gpGraphicsManager->shader_Blur.setInt("horizontal", horizontal);

		glActiveTexture(GL_TEXTURE15);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? gpGraphicsManager->colorBuffers[1] : gpGraphicsManager->pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
		//glBindTexture(GL_TEXTURE_2D, gpGraphicsManager->colorBuffers[1]);  // bind texture of other framebuffer (or scene if first iteration)

		GLint location_2 = glGetUniformLocation(gpGraphicsManager->shader_Blur.ID, "image");
		glUniform1i(location_2, 15);

		glBindVertexArray(VAO_Quad);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, VAO_HDR);
	Use_Shader3D_Raw(gpGraphicsManager->shader3D_Raw);
	Update3D_Raw(gpGraphicsManager->shader3D_Raw);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Use_Shader_Particle(gpGraphicsManager->shader_Particle);
	Update_Particle(gpGraphicsManager->shader_Particle);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	/////////////////////////////

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	///////////////////

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gpGraphicsManager->shader_HDR.use();
	
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, gpGraphicsManager->colorBuffers[0]);
	GLint location = glGetUniformLocation(gpGraphicsManager->shader_HDR.ID, "hdrBuffer");
	glUniform1i(location, 13);

	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, gpGraphicsManager->pingpongColorbuffers[!horizontal]);
	GLint location_1 = glGetUniformLocation(gpGraphicsManager->shader_HDR.ID, "bloomBlur");
	glUniform1i(location_1, 14);

	glBindVertexArray(VAO_Quad);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

#ifdef _DEBUG
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // _DEBUG
}

float Distance_Check(glm::vec3 A, glm::vec3 B)
{
	return fabs((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y) + (A.z - B.z)*(A.z - B.z));
}

void RenderSystem::Update2D()
{
	glm::mat4 projection2D = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -1.0f, 1.0f);
	
	Transform* pTr;
	Sprite *pSp;

	glUseProgram(gpGraphicsManager->shader2D.ID);
	Component* c = gpComponentManager->mSprite->GetHead();
	while (nullptr != c) {
		pSp = static_cast<Sprite*>(c);
		pTr = pSp->mpOwner->GetComponent<Transform>();
		if (pTr != nullptr) {
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, glm::vec3(pTr->mPos.x, pTr->mPos.y, 0.0f));
			model = glm::scale(model, glm::vec3(pTr->mScale.x, pTr->mScale.y, 0.0f));
			
			gpGraphicsManager->shader2D.setMat4(projection_location, projection2D);
			//gpGraphicsManager->shader2D.setMat4(view_location, gpCameraManager->GetViewMatrix());
			gpGraphicsManager->shader2D.setMat4(model_location, model);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, pSp->mTextureID);
			GLint g = glGetUniformLocation(gpGraphicsManager->shader2D.ID, "uniformTexture");
			glUniform1i(g, 2);
			
			glBindVertexArray(pSp->mVAO);
	
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		c = c->GetNext();
	}
}
//
void RenderSystem::Update3D_Model(Shader ourShader)
{
	glEnable(GL_TEXTURE_2D);	
	Model *pMd = nullptr;
	BackgroundModel *pBMd = nullptr;
	Body* pbody = nullptr;
	Transform *pTr = nullptr;
	

	glActiveTexture(GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_2D, gpGraphicsManager->frame_buffer_texture_buffer);
	GLint location = glGetUniformLocation(ourShader.ID, "shadow_sampler");
	glUniform1i(location, 1);
	glActiveTexture(GL_TEXTURE1);
	Component* c = gpComponentManager->mModel->GetHead();
	while (nullptr != c) {
		pMd = static_cast<Model*>(c);
		pTr = pMd->mpOwner->GetComponent<Transform>();
		if (pMd->light)
		{
			ourShader.setInt("light_on", 1);
		}
		else
		{

			ourShader.setInt("light_on", 0);
		}
	//	ourShader.setVec4("Color_Model", pMd->mColor);
		if (pMd->animation)
		{

			
 			ourShader.setInt("animation_State", 1);
			ourShader.setInt("total_Bones", pMd->m_num_bones);
			

			for (int i = 0; i < pMd->m_num_bones; i++) // get location all matrices of bones
			{
				string name = "bones[" + to_string(i) + "]";// name like in shader

				pMd->m_bone_location[i] = glGetUniformLocation(ourShader.ID, name.c_str());

				glUniformMatrix4fv(pMd->m_bone_location[i], 1, GL_TRUE, (const GLfloat*)&pMd->transforms[i]);
			}


		}
		else
		{
			ourShader.setInt("animation_State", 0);
		}


		ourShader.setVec3("player_pos", pTr->mPos);
	//	pMd->mCol =glm::normalize(pMd->mCol);

		float color_divide = pMd->mCol.x+ pMd->mCol.y+pMd->mCol.z;

	#ifdef  _DEBUG
		if(pMd->isSelected)
			ourShader.setVec4("Color_Model", pMd->mCol.x, 5.0f, pMd->mCol.z, 1.0f);
		else
			ourShader.setVec4("Color_Model", pMd->mCol.x, pMd->mCol.y , pMd->mCol.z , 1.0f);
	#else
		ourShader.setVec4("Color_Model", pMd->mCol.x, pMd->mCol.y, pMd->mCol.z, 1.0f);
	#endif //  _DEBUG

		ourShader.setMat4(model_location, *(pMd->mModelMatrix));
		for (unsigned int i = 0; i < pMd->mMeshes.size(); i++)
			pMd->mMeshes[i].Draw(ourShader);
		c = c->GetNext();
	}
	
}

//void RenderSystem::Update3D_Background_Model(Shader ourShader)
//{
//	glEnable(GL_TEXTURE_2D);
//	Model* pMd = nullptr;
//	BackgroundModel* pBMd = nullptr;
//	Body* pbody = nullptr;
//
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, gpGraphicsManager->frame_buffer_texture_buffer);
//	GLint location = glGetUniformLocation(ourShader.ID, "shadow_sampler");
//	glUniform1i(location, 1);
//	glActiveTexture(GL_TEXTURE1);
//
//	Component* cb = gpComponentManager->mBackground_Model->GetHead();
//	while (nullptr != cb) {
//		pBMd = static_cast<BackgroundModel*>(cb);
//		ourShader.setMat4(model_location, *(pBMd->mBackground_ModelMatrix));
//		for (unsigned int i = 0; i < pBMd->mMeshes.size(); i++)
//			pBMd->mMeshes[i].Draw(ourShader);
//		cb = cb->GetNext();
//	}
//}

void RenderSystem::Update3D_Stencil_Toon_Model_Stencil(Shader ourShader)
{

	glEnable(GL_TEXTURE_2D);
	Stencil_Toon_Model *pSTMd = nullptr;
	BackgroundModel *pBMd = nullptr;
	Body* pbody = nullptr;


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gpGraphicsManager->frame_buffer_texture_buffer);
	GLint location = glGetUniformLocation(ourShader.ID, "shadow_sampler");
	glUniform1i(location, 1);
	glActiveTexture(GL_TEXTURE1);
	Component* c = gpComponentManager->mStencil_Toon_Model->GetHead();
	while (nullptr != c) {
		pSTMd = static_cast<Stencil_Toon_Model*>(c);

		if (pSTMd->animation)
		{
			
			std::vector<aiMatrix4x4> transforms;
			ourShader.setInt("animation_State", 1);
			ourShader.setInt("total_Bones", pSTMd->m_num_bones);

		

			for (int i = 0; i < pSTMd->m_num_bones; i++) // get location all matrices of bones
			{
				string name = "bones[" + to_string(i) + "]";// name like in shader

				pSTMd->m_bone_location[i] = glGetUniformLocation(ourShader.ID, name.c_str());



				glUniformMatrix4fv(pSTMd->m_bone_location[i], 1, GL_TRUE, (const GLfloat*)&pSTMd->transforms[i]);

			}


		}
		else
		{
			ourShader.setInt("animation_State", 0);
		}

		ourShader.setMat4(model_location, *(pSTMd->mStencil_Matrix));
		for (unsigned int i = 0; i < pSTMd->mMeshes.size(); i++)
			pSTMd->mMeshes[i].Draw(ourShader);
		c = c->GetNext();
	}
}

void RenderSystem::Update3D_Stencil_Toon_Model(Shader ourShader)
{

	glEnable(GL_TEXTURE_2D);
	Stencil_Toon_Model *pSTMd = nullptr;
	BackgroundModel *pBMd = nullptr;
	Body* pbody = nullptr;
	Transform* pTr;
	World* pWorld;
	Component* cb = gpComponentManager->mWorld->GetHead();

	pWorld = static_cast<World*>(cb);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gpGraphicsManager->frame_buffer_texture_buffer);
	GLint location = glGetUniformLocation(ourShader.ID, "shadow_sampler");
	glUniform1i(location, 1);
	glActiveTexture(GL_TEXTURE1);
	Component* c = gpComponentManager->mStencil_Toon_Model->GetHead();
	while (nullptr != c) {
		pSTMd = static_cast<Stencil_Toon_Model*>(c);
		if (pSTMd->light)
		{
			ourShader.setInt("light_on", 1);
		}
		else
		{

			ourShader.setInt("light_on", 0);
		}
		///////////////////
		if (pSTMd->animation)
		{

			std::vector<aiMatrix4x4> transforms;
			ourShader.setInt("animation_State", 1);
			ourShader.setInt("total_Bones", pSTMd->m_num_bones);
			if (0 != pSTMd->transforms.size())
			{

				for (int i = 0; i < pSTMd->m_num_bones; i++) // get location all matrices of bones
				{
					string name = "bones[" + to_string(i) + "]";// name like in shader

					pSTMd->m_bone_location[i] = glGetUniformLocation(ourShader.ID, name.c_str());



					glUniformMatrix4fv(pSTMd->m_bone_location[i], 1, GL_TRUE, (const GLfloat*)&pSTMd->transforms[i]);


				}

			}

			else
			{
				ourShader.setInt("animation_State", 0);
			}
			/////////////////
			pTr = pSTMd->mpOwner->GetComponent<Transform>();

			ourShader.setVec4("Color_Model", pSTMd->mCol.x, pSTMd->mCol.y, pSTMd->mCol.z, 1.0f);
			float dis = 9999;
			std::list<Light*>::iterator it;
			for (it = pWorld->light_list.begin(); it != pWorld->light_list.end(); ++it)
			{


				if (dis > Distance_Check(pTr->mPos, (*it)->point_Light_Position))
				{
					pWorld->light_Closest = *it;
					dis = Distance_Check(pTr->mPos, (*it)->point_Light_Position);
				}
			}
			ourShader.setVec3("closestLight", pWorld->light_Closest->point_Light_Position);

			ourShader.setMat4(model_location, *(pSTMd->mModelMatrix));
			for (unsigned int i = 0; i < pSTMd->mMeshes.size(); i++)
				pSTMd->mMeshes[i].Draw(ourShader);
			c = c->GetNext();
		}
	}

}

void RenderSystem::Update3D_Debug(Shader ourShader)
{

	Collider *pCol;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Component* c = gpComponentManager->mCollider->GetHead();
	while (nullptr != c) {
		pCol = static_cast<Collider*>(c);
		ourShader.setMat4(model_location, *(pCol->mDebugMatrix));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		c = c->GetNext();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderSystem::Update3D_Raw(Shader ourShader)
{
	Transform* pTr;
	RawModel *pRMd;

	Component* c = gpComponentManager->mRawModel->GetHead();
	while (nullptr != c) {
		pRMd = static_cast<RawModel*>(c);
		pTr = pRMd->mpOwner->GetComponent<Transform>();
		if (pTr != nullptr) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pTr->mPos);
			model = glm::rotate(model, glm::radians(pTr->mRotate), glm::vec3(0, 1, 0.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(pTr->mScale));

			ourShader.setMat4(model_location, model);

			for (unsigned int i = 0; i < pRMd->mMeshes.size(); i++)
				pRMd->mMeshes[i].Draw(ourShader);
		}
		c = c->GetNext();
	}
}

void RenderSystem::Update_Lights(Shader ourShader)
{

	World* pWorld = nullptr;
	Light* pLight = nullptr;

	Component* c = gpComponentManager->mWorld->GetHead();

	pWorld = static_cast<World*>(c);

	if (pWorld == nullptr)
	{
		return;
	}

	int point_light_count = 0;
	int spot_light_count = 0;
	int directional_light_count = 0;
	std::list<Light*>::iterator it;
	for (it = pWorld->light_list.begin(); it != pWorld->light_list.end(); ++it)
	{
		if ((*it)->type == 0)
		{
			ourShader.setVec3(std::string("pointLights[") + std::to_string(point_light_count) + std::string("].position"), (*it)->point_Light_Position);
			ourShader.setFloat(std::string("pointLights[") + std::to_string(point_light_count) + std::string("].constant"), (*it)->constant);
			ourShader.setFloat(std::string("pointLights[") + std::to_string(point_light_count) + std::string("].linear"), (*it)->linear);
			ourShader.setFloat(std::string("pointLights[") + std::to_string(point_light_count) + std::string("].quadratic"), (*it)->quadratic);
			ourShader.setVec3(std::string("pointLights[") + std::to_string(point_light_count) + std::string("].ambient"), (*it)->ambient);
			ourShader.setVec3(std::string("pointLights[") + std::to_string(point_light_count) + std::string("].diffuse"), (*it)->diffuse);
			ourShader.setVec3(std::string("pointLights[") + std::to_string(point_light_count) + std::string("].specular"), (*it)->specular);
			point_light_count += 1;
		}
		else if ((*it)->type == 1)
		{
			ourShader.setVec3(std::string("spotLight[") + std::to_string(spot_light_count) + std::string("].position"), (*it)->point_Light_Position);

			ourShader.setVec3(std::string("spotLight[") + std::to_string(spot_light_count) + std::string("].direction"), (*it)->direction);
			ourShader.setFloat(std::string("spotLight[") + std::to_string(spot_light_count) + std::string("].cutOff"), glm::cos(glm::radians((*it)->cutOff)));
			ourShader.setFloat(std::string("spotLight[") + std::to_string(spot_light_count) + std::string("].outerCutOff"), glm::cos(glm::radians((*it)->outerCutOff)));

			ourShader.setFloat(std::string("spotLight[") + std::to_string(spot_light_count) + std::string("].constant"), (*it)->constant);
			ourShader.setFloat(std::string("spotLight[") + std::to_string(spot_light_count) + std::string("].linear"), (*it)->linear);
			ourShader.setFloat(std::string("spotLight[") + std::to_string(spot_light_count) + std::string("].quadratic"), (*it)->quadratic);
			ourShader.setVec3(std::string("spotLight[") + std::to_string(spot_light_count) + std::string("].ambient"), (*it)->ambient);

			ourShader.setVec3(std::string("spotLight[") + std::to_string(spot_light_count) + std::string("].diffuse"), (*it)->diffuse);
			ourShader.setVec3(std::string("spotLight[") + std::to_string(spot_light_count) + std::string("].specular"), (*it)->specular);

			spot_light_count += 1;

		}
		else
		{


			ourShader.setVec3(std::string("dirLight[") + std::to_string(spot_light_count) + std::string("].direction"), (*it)->direction);
			ourShader.setVec3(std::string("dirLight[") + std::to_string(spot_light_count) + std::string("].ambient"), (*it)->ambient);

			ourShader.setVec3(std::string("dirLight[") + std::to_string(spot_light_count) + std::string("].diffuse"), (*it)->diffuse);
			ourShader.setVec3(std::string("dirLight[") + std::to_string(spot_light_count) + std::string("].specular"), (*it)->specular);

			*lightView = glm::lookAt((*it)->point_Light_Position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			*shadow_matrix = (*projection_light) * (*lightView);
			directional_light_count += 1;
		}



	}

	ourShader.setInt("total_Point_Light", point_light_count);
	ourShader.setInt("total_Spot_Light", spot_light_count);
	ourShader.setInt("total_Directional_Light", directional_light_count);
}

void RenderSystem::Update_Particle(Shader ourShader)
{
	////////////////////////////////////
	//////////////////////
	ParticleGenerator* pPartGen;



	Component* c = gpComponentManager->mParticleGenerator->GetHead();
	while (nullptr != c) {
		pPartGen = static_cast<ParticleGenerator*>(c);
		unsigned int amount = pPartGen->particle_list.size();
		if (amount == 0)
		{
			c = c->GetNext();
			continue;
		}
				

		glBindVertexArray(pPartGen->mVAO);
		
							

		glBindBuffer(GL_ARRAY_BUFFER, pPartGen->vertexMatrix);
		glBufferSubData(GL_ARRAY_BUFFER, 0, amount * sizeof(glm::mat4), glm::value_ptr(pPartGen->model_Matrix[0]));
							
		

		glBindBuffer(GL_ARRAY_BUFFER, pPartGen->vertexColor);
		glBufferSubData(GL_ARRAY_BUFFER, 0, amount * sizeof(glm::vec4), glm::value_ptr(pPartGen->color_Particles[0]));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

							
							
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, pPartGen->mTextureID);
		GLint g = glGetUniformLocation(ourShader.ID, "sprite");
		glUniform1i(g, 9);

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, amount);



		c = c->GetNext();
	}
}

RenderSystem::~RenderSystem()
{
	_aligned_free(shadow_matrix);
	_aligned_free(lightView);
	_aligned_free(projection_light);
}

void RenderSystem::Use_Shader3D(Shader ourShader)
{
	glUseProgram(ourShader.ID);
	ourShader.setInt("stencil_toon", 0);
	ourShader.setInt("animation_State", 0);
	ourShader.setMat4(shadow_Matrix, *shadow_matrix);
	ourShader.setMat4(projection_location, gpCameraManager->GetProjectionMatrix());
	ourShader.setMat4(view_location, gpCameraManager->GetViewMatrix());
	ourShader.setVec3("camera_pos", gpCameraManager->mPosition);
	Update_Lights(ourShader);
}

void RenderSystem::Use_Shader3D_Stencil_Toon(Shader ourShader)
{
	glUseProgram(ourShader.ID);
	ourShader.setInt("animation_State", 0);
	ourShader.setMat4(shadow_Matrix, *shadow_matrix);
	ourShader.setMat4(projection_location, gpCameraManager->GetProjectionMatrix());
	ourShader.setMat4(view_location, gpCameraManager->GetViewMatrix());
	ourShader.setVec3("camera_pos", gpCameraManager->mPosition);
	ourShader.setInt("stencil_toon", 1);
	Update_Lights(ourShader);
}

void RenderSystem::Use_Shader3D_Debug(Shader ourShader)
{
	glUseProgram(ourShader.ID);
	ourShader.setMat4(projection_location, gpCameraManager->GetProjectionMatrix());
	ourShader.setMat4(view_location, gpCameraManager->GetViewMatrix());
}

void RenderSystem::Use_Shader3D_Raw(Shader ourShader)
{
	glUseProgram(ourShader.ID);
	ourShader.setInt("animation_State", 0);
	ourShader.setMat4(shadow_Matrix, *shadow_matrix);
	ourShader.setMat4(projection_location, gpCameraManager->GetProjectionMatrix());
	ourShader.setMat4(view_location, gpCameraManager->GetViewMatrix());
}

void RenderSystem::Use_Shader3D_Shadow(Shader ourShader)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gpGraphicsManager->frame_buffer);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(ourShader.ID);
	ourShader.setMat4(projection_location, *projection_light);
	ourShader.setMat4(view_location, *lightView);

}

void RenderSystem::End_Use_Shader3D_Shadow()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, (GLsizei)SCR_WIDTH, (GLsizei)SCR_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void RenderSystem::Use_Shader_Particle(Shader ourShader)
{
	glUseProgram(ourShader.ID);
	ourShader.setMat4(9, gpCameraManager->GetProjectionMatrix());
	ourShader.setMat4(8, gpCameraManager->GetViewMatrix());
}

void RenderSystem::Use_Stencil_Shader(Shader ourShader)
{
	glUseProgram(ourShader.ID);
	ourShader.setInt("animation_State", 0);
	ourShader.setMat4(shadow_Matrix, *shadow_matrix);
	ourShader.setMat4(projection_location, gpCameraManager->GetProjectionMatrix());
	ourShader.setMat4(view_location, gpCameraManager->GetViewMatrix());
}