/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shashwat Pandey, shashwat.pandey, 60003718
- End Header --------------------------------------------------------*/

#pragma once

enum KEYBOARD_KEYS {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	JUMP,
	Z,
	X,
	N,
	D,
	ESC,
	

	MOUSE_LEFT,
	MOUSE_RIGHT,
	
	R,
	E,
	ONE,
	TWO,
	LCTRL,
	LSHIFT,
	S,
	Y,
	W,
	A,
	Q,
	O,
	P,
	ZERO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	DELETE1,
	F,
	ENTER,
	F1,
	F11,
	M,
	F12,
	RCTRL,
	L,
	K,
	J,
	I,

	NUM
};

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Update(GLFWwindow*);

	bool isPressed(unsigned int KeyScanCode);
	bool isTriggered(unsigned int KeyScanCode);
	bool isReleased(unsigned int KeyScanCode);

	inline bool isLeftClicked() { return leftClicked; }
	inline bool isRightClicked() { return rightClicked; }
	
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	static glm::vec2 getCursorCoord() { return cursorCoord; }

private:
	void UpdateStatesForKeyboardKeys(GLFWwindow*);
	void UpdateStatesForJoystick(GLFWwindow*);
	void SetKeyStates(int state, KEYBOARD_KEYS key);
	void SetJoyStickKeyStates(int state, KEYBOARD_KEYS key);

public:
	bool isJoyStick;
	
private:
	int keyCodes[NUM];
	
	int joyStickId;

	bool leftClicked;
	bool preLeftClicked;

	bool rightClicked;

	static glm::vec2 cursorCoord;

	std::unordered_map<KEYBOARD_KEYS, bool> keyStatePressed;
	std::unordered_map<KEYBOARD_KEYS, bool> keyStateReleased;
	std::unordered_map<KEYBOARD_KEYS, bool> keyStateTriggered;
	
	std::unordered_map<KEYBOARD_KEYS, bool> joyStickKeyStatePressed;
	std::unordered_map<KEYBOARD_KEYS, bool> joyStickKeyStateReleased;
	std::unordered_map<KEYBOARD_KEYS, bool> joyStickKeyStateTriggered;
};