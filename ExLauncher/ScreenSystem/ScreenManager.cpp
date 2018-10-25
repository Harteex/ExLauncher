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

#include "ScreenManager.h"
#include "Screen.h"
#include "../Screens/ScreenError.h"

using namespace std;

ScreenManager::ScreenManager()
{
	input = NULL;
	displaySize.w = 0;
	displaySize.h = 0;
	lastError = "No error";
	exit = false;
	isExiting = false;
}

ScreenManager::~ScreenManager()
{
	if (input != NULL)
		delete input;
}

bool ScreenManager::Init(SDL_Renderer* renderer)
{
	input = new InputState();
	resourceManager.SetRenderer(renderer);
	graphics.SetRenderer(renderer);

	SDL_Rect r;
	SDL_RenderGetViewport(renderer, &r);
	displaySize.w = r.w;
	displaySize.h = r.h;

	filesystemWatchManager.Init(this, &appManager);

	return true;
}

bool ScreenManager::LoadGlobalResources()
{
	if (GetResourceManager()->GetTTFFont("semibold", 18) == nullptr)
	{
		lastError = "Failed to load data/fonts/OpenSans-Semibold.ttf";
		return false;
	}

	if (GetResourceManager()->GetTTFFont("semibold", 12) == nullptr)
	{
		lastError = "Failed to load data/fonts/OpenSans-Semibold.ttf";
		return false;
	}

	if (GetResourceManager()->LoadImage("appIconDefault", "data/graphics/appIconDefault.png") == nullptr)
	{
		lastError = "Failed to load data/graphics/appIconDefault.png";
		return false;
	}

	return true;
}

Graphics& ScreenManager::GetGraphics()
{
	return graphics;
}

Size ScreenManager::GetDisplaySize()
{
	return displaySize;
}

ResourceManager* ScreenManager::GetResourceManager()
{
	return &resourceManager;
}

/*NetworkManager* ScreenManager::GetNetworkManagerManager()
{
	return &networkManager;
}*/

AppManager* ScreenManager::GetAppManager()
{
	return &appManager;
}

ThemeManager * ScreenManager::GetThemeManager()
{
	return &themeManager;
}

void ScreenManager::Draw()
{
	if (HasExit())
		return;

	graphics.Clear(Color(0, 0, 0));

	for (list<Screen *>::iterator i = screens.end(); i != screens.begin(); )
	{
		i--;

		if ((*i)->GetScreenState() == Hidden || (*i)->HasExited())
			continue;

		(*i)->DrawHelper(graphics);
	}

	graphics.Present();
}

void ScreenManager::Update()
{
	filesystemWatchManager.Update();

	input->Update();

	while(SDL_PollEvent(&event)) 
	{      
		switch (event.type) 
		{
		case SDL_QUIT:
			Exit();
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			input->KeyStateChanged(&event.key);
			break;
		}
	}

	if (screens.empty())
	{
		Exit();
	}

	if (isExiting)
	{
		DeleteAllScreens();
		GetAppManager()->SaveRecentList(); // FIXME error check
		GetAppManager()->UnloadApps();
		GetResourceManager()->UnloadContent();
		exit = true;
	}
	else
	{
		bool otherScreenHasFocus = false;
		bool coveredByOtherScreen = false;

		for (list<Screen *>::iterator i = screens.begin(); i != screens.end(); )
		{
			if ((*i)->HasExited())
			{
				list<Screen *>::iterator temp = i;
				delete *temp;
				i = screens.erase(temp); // FIXME TEST THIS!!!
				continue;
			}

			(*i)->Update(otherScreenHasFocus, coveredByOtherScreen);

			if ((*i)->GetScreenState() == TransitionOn || (*i)->GetScreenState() == Active)
			{
				if (!otherScreenHasFocus)
				{
					(*i)->HandleInput(input);

					otherScreenHasFocus = true;
				}

				// If this screen is active and is a non-popup, inform following screens that they are covered by it.
				if ((*i)->IsPopup() == false)
					coveredByOtherScreen = true;
			}

			i++;
		}
	}
}

void ScreenManager::AddScreen(Screen * screen)
{
	screen->SetScreenManager(this);

	if (!screen->Initialize(graphics))
	{
		string errMsg = "Failed to initialize screen: ";
		errMsg = errMsg + screen->GetLastError();
		delete screen;

		std::cout << errMsg << std::endl;

		ScreenError* errorScreen = new ScreenError(errMsg);
		errorScreen->SetScreenManager(this);
		if (!errorScreen->Initialize(graphics))
			Exit();

		screens.push_front(errorScreen);
	}
	else
	{
		screens.push_front(screen);
	}
}

void ScreenManager::ExitAllScreens()
{
	for (list<Screen *>::iterator i = screens.begin(); i != screens.end(); i++)
	{
		if (!(*i)->IsPinned())
			(*i)->ExitScreen();
	}
}

void ScreenManager::DeleteAllScreens()
{
	for (list<Screen *>::iterator i = screens.begin(); i != screens.end(); i++)
	{
		delete *i;
	}
	screens.clear();
}

void ScreenManager::Exit()
{
	isExiting = true;
}

bool ScreenManager::HasExit()
{
	return exit;
}

void ScreenManager::TraceScreens()
{
	int n = 1;
	string message;

	for (list<Screen *>::iterator i = screens.begin(); i != screens.end(); i++)
	{
		message.append("Screen ");
		stringstream out;
		out << n;
		message.append(out.str());

		message.append(": ");

		switch ((*i)->GetScreenState())
		{
		case TransitionOn:
			message.append("TransitionOn\n");
			break;
		case Active:
			message.append("Active\n");
			break;
		case TransitionOff:
			message.append("TransitionOff\n");
			break;
		case Hidden:
			message.append("Hidden\n");
			break;
		default:
			message.append("Unknown screen state!\n");
		}
		n++;
	}
	//allegro_message(message.c_str());
}

void ScreenManager::SetGameKeyBindings(int* keyMappingArray, int numberOfKeys)
{
	input->InitializeGameKeys(keyMappingArray, numberOfKeys);
}

string ScreenManager::GetLastError()
{
	return lastError;
}

void ScreenManager::SendEvent(EventType type, std::string value, std::vector<std::string> args)
{
	for (Screen* screen : screens)
	{
		screen->OnEvent(NULL, type, value, args);
	}
}
