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

#include "InputState.h"
#include <string.h>

InputState::InputState()
{
	numberOfKeys = SDL_NUM_SCANCODES;
	realNumberOfKeys = SDL_NUM_SCANCODES;
	while (numberOfKeys % 4 != 0)
	{
		numberOfKeys++;
	}

	keyState = new Uint8[numberOfKeys];
	lastKeyState = new Uint8[numberOfKeys];

	memset(keyState, 0, numberOfKeys);
	memset(lastKeyState, 0, numberOfKeys);

	textInputEnabled = false;
	textInputBufferSize = 0;
	textInputBufferMaxSize = 16;
	textInputBuffer = new Uint16[textInputBufferMaxSize];

	memset(&mouseButtonState, 0, MOUSE_BUTTONS);
	memset(&lastMouseButtonState, 0, MOUSE_BUTTONS);

	mousePosition.x = 0;
	mousePosition.y = 0;
	lastMousePosition.x = 0;
	lastMousePosition.y = 0;

	gameKeyArray = NULL;

	repeatKey = -1;
	repeatKeyDown = false;
}

InputState::~InputState()
{
	delete [] keyState;
	delete [] lastKeyState;
	delete [] textInputBuffer;

	if (gameKeyArray != NULL)
		delete [] gameKeyArray;
}

bool InputState::KeyPressed(int key)
{
	return keyState[key] == 1;
}

bool InputState::KeyJustPressed(int key)
{
	return keyState[key] == 1 && lastKeyState[key] == 0;
}

bool InputState::KeyJustReleased(int key)
{
	return keyState[key] == 0 && lastKeyState[key] == 1;
}

bool InputState::KeyRepeat(int key)
{
	return KeyJustPressed(key) || (repeatKey == key && repeatKeyDown);
}

void InputState::InitializeGameKeys(int* keyMappingArray, int numberOfKeys)
{
	if (gameKeyArray != NULL)
		delete [] gameKeyArray;

	gameKeyArray = new int[numberOfKeys];

	for (int i = 0; i < numberOfKeys; i++)
	{
		gameKeyArray[i] = keyMappingArray[i];
	}
}

bool InputState::GameKeyPressed(int key)
{
	return KeyPressed(gameKeyArray[key]);
}

bool InputState::GameKeyJustPressed(int key)
{
	return KeyJustPressed(gameKeyArray[key]);
}

bool InputState::GameKeyJustReleased(int key)
{
	return KeyJustReleased(gameKeyArray[key]);
}

bool InputState::GameKeyRepeat(int key)
{
	return KeyRepeat(gameKeyArray[key]);
}

bool InputState::MouseDown(MouseButton button)
{
	return mouseButtonState[button] == 1;
}

bool InputState::MouseJustClicked(MouseButton button)
{
	return mouseButtonState[button] == 1 && lastMouseButtonState[button] == 0;
}

bool InputState::MouseJustReleased(MouseButton button)
{
	return mouseButtonState[button] == 0 && lastMouseButtonState[button] == 1;
}

Position InputState::MousePosition()
{
	return mousePosition;
}

Position InputState::MouseLastPosition()
{
	return lastMousePosition;
}

void InputState::Update()
{
	// Copy key state
	memcpy(lastKeyState, keyState, numberOfKeys);

	// Copy mouse button state
	memcpy(&lastMouseButtonState, &mouseButtonState, MOUSE_BUTTONS);

	// Copy mouse position
	lastMousePosition.x = mousePosition.x;
	lastMousePosition.y = mousePosition.y;

	// Read mouse state
	Uint8 tempMouseButtonState = SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
	mouseButtonState[MouseButtonLeft] = (tempMouseButtonState & SDL_BUTTON(1)) > 0 ? 1 : 0;
	mouseButtonState[MouseButtonMiddle] = (tempMouseButtonState & SDL_BUTTON(2)) > 0 ? 1 : 0;
	mouseButtonState[MouseButtonRight] = (tempMouseButtonState & SDL_BUTTON(3)) > 0 ? 1 : 0;

	// Clear textinput buffer
	textInputBufferSize = 0;

	repeatKeyDown = false;
}

void InputState::KeyStateChanged(SDL_KeyboardEvent* sdlkey)
{
	if (sdlkey->repeat == 0)
	{
		keyState[sdlkey->keysym.scancode] = (sdlkey->type == SDL_KEYDOWN) ? 1 : 0;
	}
	else
	{
		repeatKey = sdlkey->keysym.scancode;
		repeatKeyDown = true;
	}

	/*if (textInputEnabled && sdlkey->type == SDL_KEYDOWN)
	{
		if (textInputBufferSize < textInputBufferMaxSize && sdlkey->keysym.unicode >= 0x20)
		{
			textInputBuffer[textInputBufferSize++] = sdlkey->keysym.unicode;
		}
	}*/
}

void InputState::EnableTextInput(bool enabled)
{
	/*if (textInputEnabled == enabled)
		return;

	textInputEnabled = enabled;
	SDL_EnableUNICODE(enabled ? 1 : 0);*/
}

Uint16 InputState::GetNextInputCharacter()
{
	if (textInputBufferSize == 0)
		return 0;

	return textInputBuffer[--textInputBufferSize];
}

bool InputState::HasMoreInputCharacters()
{
	return textInputBufferSize > 0;
}
