/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "GameStateManager.h"

#include "CameraManager.h"
#include "CollisionManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "SystemManager.h"
#include "FrameRateController.h"
#include "ObjectFactory.h"
#include "ResourceManager.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "AudioManager.h"
#include "GameplayManager.h"
#include "../Utilities/Reflection.h"
#include "../Utilities/Editor.h"
#include "../Utilities/RayCast.h"
#include "../Shaders/stb_image.h"
#include "../Systems/ControlSystem.h"

float SCR_WIDTH;
float SCR_HEIGHT;
int isNotMinimized = 1;

std::string glsl_version;
GLFWwindow* gpWindow;
ComponentManager* gpComponentManager;
EntityManager* gpEntityManager;
InputManager* gpInputManager;
FrameRateController* gpFrameRateController;
SystemManager * gpSystemManager;
ObjectFactory* gpObjectFactory;
ResourceManager* gpResourceManager;
CollisionManager* gpCollisionManager;
GraphicsManager* gpGraphicsManager;
SceneManager* gpSceneManager;
Reflection* gpReflection;
EventManager* gpEventManager;
AudioManager* gpAudioManager;
GameplayManager* gpManager;
CameraManager* gpCameraManager;
Editor* gpEditor;
RayCast* gpRayCast;

bool gEditMode = false;
bool isPaused = false;
bool gameStarted = false;
bool flipScreenMode = false;
extern bool isAllMute;
GLFWmonitor* monitor;

void processInput(GLFWwindow *window);
void pause_callback(GLFWwindow *window, int minimized);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void GameStateManager::Init()
{
	Logger::Init();
	// Below are the 2 examples...
	// Here FlyEngine is name of the logger and it will 
	// write logger calls like LOG_INFO, LOG_WARN... into Log File
	LOG_INFO("FlyEngine", "Engine Started:");
	//Use this Logger to Print on Console Screen
	LOG_ADD("ConsoleLogger");

	SetUpGl();

	// Initialize all Managers
	gpResourceManager = new ResourceManager(); // keep this first
	gpCameraManager = new CameraManager();
	gpSystemManager = new SystemManager();	   // keep this before component manager : dependency- body has gLuaState
	gpComponentManager = new ComponentManager();
	gpAudioManager = new AudioManager();
	gpEntityManager = new EntityManager();
	gpInputManager = new InputManager();
	gpFrameRateController = new FrameRateController(60);
	gpObjectFactory = new ObjectFactory();
	gpSceneManager = new SceneManager();
	gpCollisionManager = new CollisionManager();
	gpReflection = new Reflection();
	gpEventManager = new EventManager();
	gpManager = new GameplayManager();
	gpEditor = new Editor();
	gpRayCast = new RayCast();
}

static void glfwError(int id, const char* description)
{
	std::cout << description << std::endl;
}


void GameStateManager::SetUpGl()
{
	glfwSetErrorCallback(&glfwError);
	glfwInit();
	monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_AUTO_ICONIFY, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);	
	
	float enforcedRatio = 16.0f / 9.0f;
	SCR_WIDTH = (float)mode->width;
	SCR_HEIGHT = (float)mode->height;
	if (SCR_WIDTH / SCR_HEIGHT < enforcedRatio) {
		SCR_HEIGHT = SCR_HEIGHT * (SCR_WIDTH / SCR_HEIGHT) / enforcedRatio;
	}

	// change first NULL argument to monitor to run in full screen mode
	// BEWARE:: if an exception is thrown the window cannot be closed or minimized
	gpWindow = glfwCreateWindow((int)SCR_WIDTH, (int)SCR_HEIGHT, "Robotest", NULL, NULL);
	
	// window icon
	int nrChannels;
	GLFWimage icon;
	icon.pixels = stbi_load("Resources/Assets/Images/fireflies_icon.png", &icon.width, &icon.height, &nrChannels, 0);
	if(icon.pixels)
		glfwSetWindowIcon(gpWindow, 1, &icon);
	stbi_image_free(icon.pixels);

#ifndef _DEBUG
	glfwSetWindowMonitor(gpWindow, monitor, 0, 0, (int)SCR_WIDTH, (int)SCR_HEIGHT, 60);
#endif // _DEBUG

	if (gpWindow == NULL)
	{
		glfwTerminate();
		ASSERT("FlyEngine", nullptr, "File::GameStateManager Function::SetUpGI Details::Failed to Create GFLW window");
		return;
	}
	glfwMakeContextCurrent(gpWindow);
	glfwSetWindowFocusCallback(gpWindow, pause_callback);
	glfwSetFramebufferSizeCallback(gpWindow, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ASSERT("FlyEngine", nullptr, "File::GameStateManager Function::SetUpGI Details::Failed to initialize GLAD");
		return;
	}	
}

void GameStateManager::SafeDelete()
{
	glfwTerminate();
	delete gpComponentManager;
	delete gpEntityManager;
	delete gpInputManager;
	delete gpFrameRateController;
	delete gpSystemManager;
	delete gpObjectFactory;
	delete gpResourceManager;
	delete gpCollisionManager;
	delete gpGraphicsManager;
	delete gpSceneManager;
	delete gpReflection;
	delete gpEventManager;
	delete gpAudioManager;
	delete gpManager;
	delete gpEditor;
	delete gpCameraManager;
	delete gpRayCast;
}

void GameStateManager::StartEngine()
{
	gpGraphicsManager = new GraphicsManager();
	gpSystemManager->Init();

	//int refChannel = gpAudioManager->Play("Win.wav", -1);//play song in loop infinite
	//gpAudioManager->SetPan(refChannel ,0.0f);// changing 2d orientation lef/right speakers

#ifdef _DEBUG
	// ImGui Init
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(gpWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());
#endif

	while (!glfwWindowShouldClose(gpWindow)) {
		if (isNotMinimized)
			Update();
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		glfwPollEvents();
	}

// shutting down engine
#ifdef _DEBUG
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
#endif

	glfwDestroyWindow(gpWindow);
	glfwTerminate();
	spdlog::drop_all();
}

void GameStateManager::Update()
{
	gEditMode = gpEditor->mEditMode;

	processInput(gpWindow);
	gpFrameRateController->FrameStart();
	float frameTime = (float)gpFrameRateController->GetFrameTime();
	gpSceneManager->Update();
	gpObjectFactory->Update();

#ifdef _DEBUG
	// ImGui Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	gpEditor->Update();		// Editor only in Debug Mode
#endif // _DEBUG

	gpAudioManager->Update(frameTime);
	gpCameraManager->Update(gpInputManager);
	gpSystemManager->Update(frameTime);

	if (!gEditMode) {
		gpEventManager->Update(frameTime);
	}

	glfwSwapBuffers(gpWindow);
	gpFrameRateController->FrameEnd();
}

void processInput(GLFWwindow *window)
{
	gpInputManager->Update(window);

	if (gpInputManager->isTriggered(F11)) {
		if (flipScreenMode) {
			glfwSetWindowMonitor(gpWindow, monitor, 0, 0, (int)SCR_WIDTH,(int) SCR_HEIGHT, 60);
		}
		else {
			glfwSetWindowMonitor(gpWindow, NULL, 5, 40, (int)SCR_WIDTH, (int)SCR_HEIGHT, 60);
		}
		flipScreenMode = !flipScreenMode;
	}

	// calls glfw directly so that ESC works even with controller
	if (gpInputManager->isTriggered(ESC)) {
		
		if (gameStarted)
		{
			if (isPaused) {
				gpSystemManager->mControlSystem->isSettingPage = false;
				isPaused = false;
				
				if ( gpSystemManager->mControlSystem->isMuteYes == false) {
					if (isAllMute == true)
						gpAudioManager->ToggleMuteAll();
				}
				else {
					if ( isAllMute == false )
						gpAudioManager->ToggleMuteAll();
				}

				for (auto e : gpSceneManager->mMenuEntity) {
					gpObjectFactory->DeleteObject(e->id);
				}
				gpSceneManager->mMenuEntity.clear();
			}
			else {
				if (gpSystemManager->mControlSystem->isMuteYes == false) {
					if (isAllMute == false)
						gpAudioManager->ToggleMuteAll();
				}
				isPaused = true;
				gpCameraManager->SnapToPosition( glm::vec3(40.0f, 9.0f, 150.0f));
				gpSystemManager->mControlSystem->ReadyMainMenu();
				//gpAudioManager->ToggleAll();
				gpSceneManager->LoadLevel("PauseMenu.json");
			}
		}
		//glfwSetWindowShouldClose(window, true);
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
void pause_callback(GLFWwindow* window, int minimized) {
	isNotMinimized = minimized;
	if (gameStarted && !isPaused) {
		isPaused = true;
		if (gpSystemManager->mControlSystem->isMuteYes == false) {
			if (isAllMute == false)
				gpAudioManager->ToggleMuteAll();
		}
		gpSceneManager->LoadLevel("PauseMenu.json");
	}
}