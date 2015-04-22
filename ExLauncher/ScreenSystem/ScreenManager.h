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
public:
	ScreenManager();
	~ScreenManager();
	void Draw();
	void Update();
	void AddScreen(Screen * screen);
	void DeleteAllScreens();
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
