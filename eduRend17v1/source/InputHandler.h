//
// Mouse & keyboard via DirectInput
//
// Courtesy of Alexander Baldwin, alexander.baldwin@mah.se
//

#pragma once
#include "stdafx.h"

enum Keys {
	Left = DIK_LEFT,
	Right = DIK_RIGHT,
	Up = DIK_UP,
	Down = DIK_DOWN,
	W = DIK_W,
	A = DIK_A,
	S = DIK_S,
	D = DIK_D,
	Q = DIK_Q,
	E = DIK_E,
	L_CTRL = DIK_LCONTROL,
	Space = DIK_SPACE,
	LShift = DIK_LSHIFT,
	P = DIK_P,
	N = DIK_N,
	T = DIK_T,
	numb1 = DIK_1,
	numb2 = DIK_2,
	numb3 = DIK_3,
};

class InputHandler {
private:
	IDirectInput8* directInput;
	IDirectInputDevice8* keyboard;
	IDirectInputDevice8* mouse;
	unsigned char keyboardState[256];
	DIMOUSESTATE mouseState, prevMouseState;
	int screenWidth, screenHeight;
	int mouseX, mouseY;

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

public:
	InputHandler();
	~InputHandler();
	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Update();
	void GetMouseLocation(int&, int&);
	bool IsKeyPressed(Keys);
	float GetMouseDeltaX();
	float GetMouseDeltaY();
};