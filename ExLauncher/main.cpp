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

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "ScreenSystem/ScreenManager.h"
#include "Screens/ScreenBackgroundImage.h"
#include "Screens/ScreenMenu.h"
#include "Screens/ScreenMessageDialog.h"
#include "Screens/ScreenError.h"
#include "Theme/ThemeManager.h"
#include "global.h"
#include "Filesystem/HomeDirectory.h"
#include <string>
#include "utils.h"
#ifdef UNIX
#include <unistd.h>
#include <stdlib.h>
#endif
#ifdef PLATFORM_GCW0
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#endif

using namespace std;

bool SDLInited = false;
SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

ScreenManager* screenManager = NULL;
bool _resetFrameSkip = false;
bool debugViewBounds = false;
bool isLauncher = false;
bool launchFailed = false;
bool dontUseThemeInConfig = false;

bool initializeSDL()
{
#ifdef PLATFORM_GCW0
	// Avoid the framebuffer being cleared on exit on the GCW Zero
	setenv("SDL_FBCON_DONT_CLEAR", "1", 0);
#endif

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL initialization error", SDL_GetError(), NULL);
		return false;
	}
	SDLInited = true;

	win = SDL_CreateWindow("ExLauncher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, SDL_WINDOW_SHOWN);
	if (win == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window", SDL_GetError(), NULL);
		return false;
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (ren == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create renderer", SDL_GetError(), NULL);
		return false;
	}

	SDL_RendererInfo renInfo;
	if (SDL_GetRendererInfo(ren, &renInfo) < 0)
	{
		std::cout << "Unable to query renderer" << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Query failed", "Unable to query renderer", NULL);
		return false;
	}

	/*if ((renInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0)
	{
		std::cout << "Renderer does not support render targets" << std::endl;
		return false;
	}*/

	if (TTF_Init() != 0)
	{
		std::cout << TTF_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "TTF initialization failed", TTF_GetError(), NULL);
		return false;
	}

	int mixFlags = MIX_INIT_OGG;
	if ((Mix_Init(mixFlags) & mixFlags) != mixFlags)
	{
		std::cout << Mix_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Mixer initialization failed", Mix_GetError(), NULL);
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		std::cout << Mix_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't open audio", Mix_GetError(), NULL);
		return false;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	return true;
}

void RestoreFb()
{
#ifdef PLATFORM_GCW0
	fb_var_screeninfo varInfo;
	int fd = open("/dev/fb0", O_RDWR);
	ioctl(fd, FBIOGET_VSCREENINFO, &varInfo);
	varInfo.yoffset = varInfo.xoffset = 0;
	ioctl(fd, FBIOPUT_VSCREENINFO, &varInfo);
#endif
}

void terminateSDL()
{
	int audioTimesOpened, frequency, channels;
	Uint16 format;
	audioTimesOpened = Mix_QuerySpec(&frequency, &format, &channels);

	while (audioTimesOpened > 0)
	{
		Mix_CloseAudio();
		audioTimesOpened--;
	}

	while (Mix_Init(0))
		Mix_Quit();

	if (TTF_WasInit())
	{
		TTF_Quit();
	}

	if (ren != NULL)
	{
		SDL_DestroyRenderer(ren);
		ren = NULL;
	}

	if (win != NULL)
	{
		SDL_DestroyWindow(win);
		win = NULL;
	}

	//if (SDL_WasInit(0) > 0) // FIXME TEST
	if (SDLInited)
	{
		SDL_Quit();
		SDLInited = false;
	}

#ifdef PLATFORM_GCW0
	unsetenv("SDL_FBCON_DONT_CLEAR");
#endif

	RestoreFb();
}

void setKeyBindings()
{
	int gameKeys[GAMEKEY_MAX];
	gameKeys[GAMEKEY_UP] = SDL_SCANCODE_UP;
	gameKeys[GAMEKEY_LEFT] = SDL_SCANCODE_LEFT;
	gameKeys[GAMEKEY_RIGHT] = SDL_SCANCODE_RIGHT;
	gameKeys[GAMEKEY_DOWN] = SDL_SCANCODE_DOWN;
	gameKeys[GAMEKEY_A] = SDL_SCANCODE_LCTRL;
	gameKeys[GAMEKEY_B] = SDL_SCANCODE_LALT;
	gameKeys[GAMEKEY_X] = SDL_SCANCODE_SPACE;
	gameKeys[GAMEKEY_Y] = SDL_SCANCODE_LSHIFT;
	gameKeys[GAMEKEY_START] = SDL_SCANCODE_RETURN;
	gameKeys[GAMEKEY_SELECT] = SDL_SCANCODE_ESCAPE;
	gameKeys[GAMEKEY_TRIGGER_L] = SDL_SCANCODE_TAB;
	gameKeys[GAMEKEY_TRIGGER_R] = SDL_SCANCODE_BACKSPACE;

	screenManager->SetGameKeyBindings(gameKeys, GAMEKEY_MAX);
}

void parseArguments(int argc, char **argv)
{
	for (int i = 1; i < argc; i++)
	{
		string arg = argv[i];

		if (arg == "--launcher")
			isLauncher = true;
		else if (arg == "--debugViewBounds")
			debugViewBounds = true;
		else if (arg.substr(0, 8) == "--theme=")
		{
			ThemeManager::SetTheme(arg.substr(8));
			dontUseThemeInConfig = true;
		}
	}
}

int main(int argc, char **argv)
{
	parseArguments(argc, argv);

mainStart:

	std::cout << "main()" << std::endl;
	std::cout << "Initializing SDL... " << std::endl;
	if (!initializeSDL())
	{
		terminateSDL();
		return 1;
	}
	std::cout << "SDL is init!!" << std::endl;

	std::cout << "Initializing ScreenManager... ";
	screenManager = new ScreenManager();
	if (!screenManager->Init(ren))
	{
		std::cout << "FAILED" << std::endl;
		terminateSDL();
		return 1;
	}
	std::cout << "OK" << std::endl;

	std::cout << "Loading resources... ";
	if (!screenManager->LoadGlobalResources())
	{
		std::cout << "FAILED" << std::endl;
		std::cout << screenManager->GetLastError() << std::endl;
		terminateSDL();
		return 1;
	}
	std::cout << "OK" << std::endl;

	screenManager->GetAppManager()->SetResourceManager(screenManager->GetResourceManager());

	if (!HomeDirectory::InitAndCreateDirectories())
	{
		std::cout << "Failed to init config / data directory paths" << std::endl;
		return 1;
	}

	measureTimeStart();
	if (!screenManager->GetAppManager()->LoadApps())
	{
		std::cout << "Failed to load apps" << std::endl;
		return 1;
	}
	double timeLoadApps = measureTimeFinish();
	std::cout << "Loaded " << screenManager->GetAppManager()->GetNumberOfApps() << " apps in " << timeLoadApps << "s." << std::endl;

	if (!screenManager->GetAppManager()->LoadRecentList())
	{
		std::cout << "Failed to load recent list" << std::endl;
		return 1;
	}

	setKeyBindings();

	screenManager->GetThemeManager()->LoadThemes();

	if (!dontUseThemeInConfig)
		screenManager->GetThemeManager()->LoadSettings();

	Theme* theme = screenManager->GetThemeManager()->GetTheme(ThemeManager::GetCurrentThemeId());
	string themeEntryPoint = "";
	if (theme != nullptr)
		themeEntryPoint = string("@theme/") + theme->GetEntryPoint();

	std::cout << "APP IS START" << std::endl;

	// TEMP TEMP TEMP!
	screenManager->GetResourceManager()->LoadImage("SnowCloseUp", "data/wallpapers/SnowCloseUp.jpg");
	screenManager->GetResourceManager()->LoadImage("GothenburgNight", "data/wallpapers/GothenburgNight.jpg");
	screenManager->GetResourceManager()->LoadImage("Bug", "data/wallpapers/Bug.jpg");
	screenManager->GetResourceManager()->LoadImage("Circuit", "data/wallpapers/Circuit.jpg");
	screenManager->GetResourceManager()->LoadImage("Leaves", "data/wallpapers/Leaves.jpg");

	screenManager->GetResourceManager()->LoadImage("IconApplications", "data/graphics/icon_applications.png");
	screenManager->GetResourceManager()->LoadImage("IconEmulators", "data/graphics/icon_emulators.png");
	screenManager->GetResourceManager()->LoadImage("IconGames", "data/graphics/icon_games.png");
	screenManager->GetResourceManager()->LoadImage("IconSettings", "data/graphics/icon_settings.png");
	
	ScreenBackgroundImage* bgScreen = new ScreenBackgroundImage();
	screenManager->AddScreen(bgScreen);

	if (themeEntryPoint.empty())
		screenManager->AddScreen(new ScreenError("Failed to load theme"));
	else
		screenManager->AddScreen(new ScreenMenu(themeEntryPoint));

	if (launchFailed)
	{
		launchFailed = false;
		ScreenMessageDialog* dialog = new ScreenMessageDialog();
		dialog->SetText("An unknown error occured while launching application.");
		screenManager->AddScreen(dialog);
	}

	bgScreen->AddImage("Circuit");
	bgScreen->AddImage("Bug");
	bgScreen->AddImage("SnowCloseUp");
	bgScreen->AddImage("GothenburgNight");
	bgScreen->AddImage("Leaves");

	_resetFrameSkip = false;
	int currentTime = SDL_GetTicks();
	double accumulator = 0;
	double frameTime = 1000.0/(double)FPS;
	int newTime;
	int deltaTime;
	int shouldDraw = 0;
	vector<string> commandToLaunchOnExit = vector<string>();

	while(!screenManager->HasExit())
	{
		newTime = SDL_GetTicks();
		deltaTime = newTime - currentTime;
		currentTime = newTime;
		accumulator += deltaTime;

#if defined(NO_FPS_LIMIT)
		accumulator = frameTime;
#endif
		if(_resetFrameSkip)
		{
			accumulator = frameTime;
			_resetFrameSkip = false;
		}

		while (accumulator >= frameTime && !screenManager->HasExit())
		{
			accumulator -= frameTime;

			screenManager->Update();
			shouldDraw = 1;
		}

		if (shouldDraw)
		{
			screenManager->Draw();
			shouldDraw = 0;
		}
		else
		{
			SDL_Delay(1);
		}
	}

	commandToLaunchOnExit = screenManager->GetAppManager()->GetCommandToLaunch();
	screenManager->GetAppManager()->ClearCommandToLaunch();

	delete screenManager;
	screenManager = NULL;
	terminateSDL();
	fflush(NULL);

	// If we should launch an app, do it now.
	if (!commandToLaunchOnExit.empty())
	{
#ifdef UNIX
		vector<const char *> args;
		args.reserve(commandToLaunchOnExit.size() + 1);
		for (auto arg : commandToLaunchOnExit) {
			args.push_back(arg.c_str());
		}
		args.push_back(nullptr);
		execvp(commandToLaunchOnExit[0].c_str(), (char* const*)&args[0]);
#endif

		// If we ended up here, execution failed. Restart.
		launchFailed = true;
		goto mainStart;
	}

	return 0;
}
