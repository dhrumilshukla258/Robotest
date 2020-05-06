/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "InputManager.h"
#include "CameraManager.h"
#include <iostream>

extern GLFWwindow* gpWindow;
extern CameraManager* gpCameraManager;
extern float SCR_WIDTH;
extern float SCR_HEIGHT;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool _leftClicked = false;
bool _rightClicked = false;

glm::vec2 InputManager::cursorCoord = glm::vec2(0);

InputManager::~InputManager()
{
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	
	//LOG_INFO("ConsoleLogger", "Mouse button pressed");
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS)
			_leftClicked = true;
		else
			_leftClicked = false;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS)
			_rightClicked = true;
		else
			_rightClicked = false;
	}
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void InputManager::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	cursorCoord = glm::vec2(xpos, ypos);

	if (_rightClicked) {
		if (firstMouse)
		{
			lastX = static_cast<float>(xpos);
			lastY = static_cast<float>(ypos);
			firstMouse = false;
		}

		float xoffset = static_cast<float>(xpos) - lastX;
		float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);

#ifdef _DEBUG
		gpCameraManager->ProcessMouseMovement(xoffset, yoffset, true);
#endif
	}
	else {
		firstMouse = true;
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	gpCameraManager->ProcessMouseScroll(static_cast<float>(yoffset));
}


void InputManager::Update(GLFWwindow* window)
{

	// GLFW gamepad support issue : Ps4 and Xbox mappings are different
	if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) {
		isJoyStick = true;
		UpdateStatesForJoystick(window);
	}
	else
		isJoyStick = false;

	UpdateStatesForKeyboardKeys(window);

	leftClicked = _leftClicked;
	rightClicked = _rightClicked;

	SetKeyStates(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT), static_cast<KEYBOARD_KEYS>(MOUSE_LEFT));
	SetKeyStates(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT), static_cast<KEYBOARD_KEYS>(MOUSE_RIGHT));

}

// joystick
// axes[0] is Left Stick X
// axes[1] is Left Stick Y
// axes[2] is Right Stick X
// axes[3] is Right Stick Y
// axes[4] is Right Trigger R2
void InputManager::UpdateStatesForJoystick(GLFWwindow* window) {
	int countAxes = 0;
	int countButtons = 0;

	/*const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &countAxes);
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &countButtons);*/

	GLFWgamepadstate state;

	if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state)) {

		//std::cout << "Left Stick X Axis: " << axes[0] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Left Stick Y Axis: " << axes[1] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Right Stick X Axis: " << axes[2] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Right Stick Y Axis: " << axes[3] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Right Trigger/R2: " << axes[4] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Right Stick: " << axes[5] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 0: " << buttons[0] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 1: " << buttons[1] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 2: " << buttons[2] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 3: " << buttons[3] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 4: " << buttons[4] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 5: " << buttons[5] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 6: " << buttons[6] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 7: " << buttons[7] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 8: " << buttons[8] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 9: " << buttons[9] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 10: " << buttons[10] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 11: " << buttons[11] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 12: " << buttons[12] << std::endl; // tested with PS4 controller connected via micro USB cable
		//std::cout << "Button 13: " << buttons[13] << std::endl; // tested with PS4 controller connected via micro USB cable


		// setting y axis
		if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > 0.5f) {
			SetJoyStickKeyStates(GLFW_PRESS, DOWN);
			SetJoyStickKeyStates(GLFW_RELEASE, UP);
		}
		else if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -0.5f) {
			SetJoyStickKeyStates(GLFW_PRESS, UP);
			SetJoyStickKeyStates(GLFW_RELEASE, DOWN);
		}
		else {
			// D-Pad
			SetJoyStickKeyStates(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP], UP);
			SetJoyStickKeyStates(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN], DOWN);

		}

		// setting x axis
		if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > 0.5f) {
			SetJoyStickKeyStates(GLFW_PRESS, RIGHT);
			SetJoyStickKeyStates(GLFW_RELEASE, LEFT);
		}
		else if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -0.5f) {
			SetJoyStickKeyStates(GLFW_PRESS, LEFT);
			SetJoyStickKeyStates(GLFW_RELEASE, RIGHT);
		}
		else {
			// D-Pad
			SetJoyStickKeyStates(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT], RIGHT);
			SetJoyStickKeyStates(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT], LEFT);

		}

		// sprint button
		if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > 0.5f) {
			SetJoyStickKeyStates(GLFW_PRESS, LSHIFT);
		}

		// grab key
		//SetJoyStickKeyStates(state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER], LCTRL);

		// Z and space both jump
		SetJoyStickKeyStates(state.buttons[GLFW_GAMEPAD_BUTTON_CROSS], JUMP);
		SetJoyStickKeyStates(state.buttons[GLFW_GAMEPAD_BUTTON_CROSS], Z);

		// shoot
		SetJoyStickKeyStates(state.buttons[GLFW_GAMEPAD_BUTTON_SQUARE], X);

		// interact
		SetJoyStickKeyStates(state.buttons[GLFW_GAMEPAD_BUTTON_TRIANGLE], E);

		// pause
		SetJoyStickKeyStates(state.buttons[GLFW_GAMEPAD_BUTTON_START], ESC);
	}
	
}

void InputManager::UpdateStatesForKeyboardKeys(GLFWwindow* gpWindow) 
{
	int state = 0;
	for (int i = 0; i < NUM; ++i) {
		if (i == MOUSE_LEFT || i == MOUSE_RIGHT) continue; // don't check for mouse here
		
		state = glfwGetKey(gpWindow, keyCodes[i]);
		SetKeyStates(state, static_cast<KEYBOARD_KEYS>(i));
		
	}
}

void InputManager::SetJoyStickKeyStates(int state, KEYBOARD_KEYS key) {
	if (state == GLFW_PRESS) {
		// check for triggered
		if (joyStickKeyStateReleased[key]) {
			joyStickKeyStateTriggered[key] = true;
		}
		else {
			joyStickKeyStateTriggered[key] = false;
		}

		joyStickKeyStatePressed[key] = true;
		joyStickKeyStateReleased[key] = false;

	}
	else if (state == GLFW_RELEASE) {
		joyStickKeyStatePressed[key] = false;
		joyStickKeyStateTriggered[key] = false;
		joyStickKeyStateReleased[key] = true;
	}

	//std::cout << key << " : " << keyStatePressed[key] << std::endl;
}

void InputManager::SetKeyStates(int state, KEYBOARD_KEYS key) {
	if (state == GLFW_PRESS) {
		// check for triggered
		if (keyStateReleased[key]) {
			keyStateTriggered[key] = true;
		}
		else {
			keyStateTriggered[key] = false;
		}

		keyStatePressed[key] = true;
		keyStateReleased[key] = false;

	}
	else if (state == GLFW_RELEASE) {
		keyStatePressed[key] = false;
		keyStateTriggered[key] = false;
		keyStateReleased[key] = true;
	}

	//std::cout << key << " : " << keyStatePressed[key] << std::endl;
}

bool InputManager::isPressed(unsigned int KeyScanCode) 
{
	return keyStatePressed[static_cast<KEYBOARD_KEYS>(KeyScanCode)] ||
		joyStickKeyStatePressed[static_cast<KEYBOARD_KEYS>(KeyScanCode)];
}

bool InputManager::isTriggered(unsigned int KeyScanCode) 
{
	return keyStateTriggered[static_cast<KEYBOARD_KEYS>(KeyScanCode)] ||
		joyStickKeyStateTriggered[static_cast<KEYBOARD_KEYS>(KeyScanCode)];
}

bool InputManager::isReleased(unsigned int KeyScanCode) {
	return keyStateReleased[static_cast<KEYBOARD_KEYS>(KeyScanCode)] &&
		joyStickKeyStateReleased[static_cast<KEYBOARD_KEYS>(KeyScanCode)];
}

InputManager::InputManager()
{



#ifdef _DEBUG

	glfwSetInputMode(gpWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#else
	glfwSetInputMode(gpWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#endif
	glfwSetCursorPosCallback(gpWindow, mouse_callback);
	//glfwSetScrollCallback(gpWindow, scroll_callback);
	glfwSetMouseButtonCallback(gpWindow, mouse_button_callback);

	// set all keycodes for keys used in game
	keyCodes[0] = GLFW_KEY_UP;
	keyCodes[1] = GLFW_KEY_DOWN;
	keyCodes[2] = GLFW_KEY_LEFT;
	keyCodes[3] = GLFW_KEY_RIGHT;
	keyCodes[4] = GLFW_KEY_SPACE;
	keyCodes[5] = GLFW_KEY_Z;
	keyCodes[6] = GLFW_KEY_X;
	keyCodes[7] = GLFW_KEY_N;
	keyCodes[8] = GLFW_KEY_D;
	keyCodes[9] = GLFW_KEY_ESCAPE;

	keyCodes[10] = GLFW_MOUSE_BUTTON_LEFT;
	keyCodes[11] = GLFW_MOUSE_BUTTON_RIGHT;
	
	keyCodes[12] = GLFW_KEY_R;
	keyCodes[13] = GLFW_KEY_E;
	keyCodes[14] = GLFW_KEY_1;
	keyCodes[15] = GLFW_KEY_2;
	keyCodes[16] = GLFW_KEY_LEFT_CONTROL;
	keyCodes[17] = GLFW_KEY_LEFT_SHIFT;
	keyCodes[18] = GLFW_KEY_S;
	keyCodes[19] = GLFW_KEY_Y;
	keyCodes[20] = GLFW_KEY_W;
	keyCodes[21] = GLFW_KEY_A;
	keyCodes[22] = GLFW_KEY_Q;
	keyCodes[23] = GLFW_KEY_O;
	keyCodes[24] = GLFW_KEY_P;
	keyCodes[25] = GLFW_KEY_0;
	keyCodes[26] = GLFW_KEY_3;
	keyCodes[27] = GLFW_KEY_4;
	keyCodes[28] = GLFW_KEY_5;
	keyCodes[29] = GLFW_KEY_6;
	keyCodes[30] = GLFW_KEY_7;
	keyCodes[31] = GLFW_KEY_8;
	keyCodes[32] = GLFW_KEY_9;
	keyCodes[33] = GLFW_KEY_DELETE;
	keyCodes[34] = GLFW_KEY_F;
	keyCodes[35] = GLFW_KEY_ENTER;
	keyCodes[F1] = GLFW_KEY_F1;
	keyCodes[37] = GLFW_KEY_F11;
	keyCodes[M] = GLFW_KEY_M;
	keyCodes[F12] = GLFW_KEY_F12;
	keyCodes[RCTRL] = GLFW_KEY_RIGHT_CONTROL;
	keyCodes[L] = GLFW_KEY_L;
	keyCodes[K] = GLFW_KEY_K;
	keyCodes[J] = GLFW_KEY_J;
	keyCodes[I] = GLFW_KEY_I;

	for (int i = 0; i < NUM; ++i) 
	{
		keyStatePressed[static_cast<KEYBOARD_KEYS>(i)] = false;
		keyStateTriggered[static_cast<KEYBOARD_KEYS>(i)] = false;
		keyStateReleased[static_cast<KEYBOARD_KEYS>(i)] = true;

		joyStickKeyStatePressed[static_cast<KEYBOARD_KEYS>(i)] = false;
		joyStickKeyStateTriggered[static_cast<KEYBOARD_KEYS>(i)] = false;
		joyStickKeyStateReleased[static_cast<KEYBOARD_KEYS>(i)] = true;
	}
}