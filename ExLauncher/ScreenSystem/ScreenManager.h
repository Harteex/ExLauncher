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

#ifndef _SCREENMANAGER_H
#define _SCREENMANAGER_H

/*********************************************/

#include <SDL.h>
#include "InputState.h"
#include "../ResourceManager.h"
/*#include "NetworkManager.h"*/
#include "../AppManager.h"
#include "../structures.h"
#include <list>
#include <iostream>
#include <sstream>

/*********************************************/

class Screen;

class ScreenManager
{
private:
	std::list<Screen *> screens;
	bool exit;
	bool isExiting;
	InputState* input;
	SDL_Renderer* renderer;
	Size displaySize;
	ResourceManager resourceManager;
	/*NetworkManager networkManager;*/
	AppManager appManager;
	SDL_Event event;
	std::string lastError;

	void DeleteAllScreens();
public:
	ScreenManager();
	~ScreenManager();
	void Draw();
	void Update();
	void AddScreen(Screen * screen);
	void ExitAllScreens();
	void Exit();
	bool HasExit();
	void TraceScreens();
	bool Init();
	bool LoadGlobalResources();
	void SetRenderer(SDL_Renderer* renderer);
	SDL_Renderer* GetRenderer();
	Size GetDisplaySize();
	ResourceManager* GetResourceManager();
	/*NetworkManager* GetNetworkManagerManager();*/
	AppManager* GetAppManager();
	void SetGameKeyBindings(int* keyMappingArray, int numberOfKeys);
	std::string GetLastError();
};

/*********************************************/

#endif
