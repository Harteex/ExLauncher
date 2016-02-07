/*
Copyright 2016 Andreas Bjerkeholt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _INPUTSTATE_H
#define _INPUTSTATE_H

/*********************************************/

#include <SDL.h>
#include "../structures.h"

/*********************************************/

#define MOUSE_BUTTONS 4
enum MouseButton {MouseButtonNone, MouseButtonLeft, MouseButtonMiddle, MouseButtonRight};
enum Buttons {ButtonA, ButtonB, ButtonX, ButtonY, ButtonStart, ButtonSelect, ButtonL, ButtonR, ButtonPower};

#define GAMEKEY_UP			0
#define GAMEKEY_LEFT		1
#define GAMEKEY_RIGHT		2
#define GAMEKEY_DOWN		3
#define GAMEKEY_A			4
#define GAMEKEY_B			5
#define GAMEKEY_X			6
#define GAMEKEY_Y			7
#define GAMEKEY_START		8
#define GAMEKEY_SELECT		9
#define GAMEKEY_TRIGGER_L	10
#define GAMEKEY_TRIGGER_R	11
#define GAMEKEY_MAX			12

/*********************************************/

class InputState
{
private:
	int numberOfKeys;
	int realNumberOfKeys;
	Uint8* keyState;
	Uint8* lastKeyState;
	bool textInputEnabled;
	int textInputBufferSize;
	int textInputBufferMaxSize;
	Uint16* textInputBuffer;
	Uint8 mouseButtonState[MOUSE_BUTTONS];
	Uint8 lastMouseButtonState[MOUSE_BUTTONS];
	Position mousePosition;
	Position lastMousePosition;
	int* gameKeyArray;
	
	int repeatKey;
	bool repeatKeyDown;
public:
	InputState();
	~InputState();
	bool KeyPressed(int key);
	bool KeyJustPressed(int key);
	bool KeyJustReleased(int key);
	bool KeyRepeat(int key);
	void InitializeGameKeys(int* keyMappingArray, int numberOfKeys);
	bool GameKeyPressed(int key);
	bool GameKeyJustPressed(int key);
	bool GameKeyJustReleased(int key);
	bool GameKeyRepeat(int key);
	bool MouseDown(MouseButton button);
	bool MouseJustClicked(MouseButton button);
	bool MouseJustReleased(MouseButton button);
	Position MousePosition();
	Position MouseLastPosition();
	void Update();
	void KeyStateChanged(SDL_KeyboardEvent* sdlkey);
	void EnableTextInput(bool enabled);
	Uint16 GetNextInputCharacter();
	bool HasMoreInputCharacters();
};

/*********************************************/

#endif
