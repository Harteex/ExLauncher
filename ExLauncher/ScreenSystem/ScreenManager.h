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
#include "../App/AppManager.h"
#include "../Theme/ThemeManager.h"
#include "../structures.h"
#include "../Graphics/Graphics.h"
#include "../Filesystem/FilesystemWatchManager.h"
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
	Graphics graphics;
	Size displaySize;
	ResourceManager resourceManager;
	/*NetworkManager networkManager;*/
	AppManager appManager;
	ThemeManager themeManager;
	FilesystemWatchManager filesystemWatchManager;
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
	bool Init(SDL_Renderer* renderer);
	bool InitFilesystemWatch();
	bool LoadGlobalResources();
	Graphics& GetGraphics();
	Size GetDisplaySize();
	ResourceManager* GetResourceManager();
	/*NetworkManager* GetNetworkManagerManager();*/
	AppManager* GetAppManager();
	ThemeManager* GetThemeManager();
	void SetGameKeyBindings(int* keyMappingArray, int numberOfKeys);
	std::string GetLastError();
	void SendEvent(EventType type, std::string value, std::vector<std::string> args);
};

/*********************************************/

#endif
