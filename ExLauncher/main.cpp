#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "ScreenSystem/ScreenManager.h"
#include "Screens/ScreenBackgroundImage.h"
#include "Screens/ScreenMenu.h"
#include "Screens/ScreenTest.h"
#include "global.h"

bool SDLInited = false;
SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

ScreenManager screenManager;

int _resetFrameSkip = 0;

bool debugViewBounds = false;

bool initializeSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
		return false;
	}
	SDLInited = true;

	win = SDL_CreateWindow("ExLauncher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, SDL_WINDOW_SHOWN);
	if (win == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		return false;
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (ren == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		return false;
	}

	SDL_RendererInfo renInfo;
	if (SDL_GetRendererInfo(ren, &renInfo) < 0)
	{
		std::cout << "Unable to query renderer" << std::endl;
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
		return false;
	}

	int mixFlags = MIX_INIT_OGG;
	if ((Mix_Init(mixFlags) & mixFlags) != mixFlags)
	{
		std::cout << Mix_GetError() << std::endl;
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		std::cout << Mix_GetError() << std::endl;
		return false;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	return true;
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
}

void setKeyBindings()
{
	int gameKeys[GAMEKEY_MAX];
	gameKeys[GAMEKEY_UP] = SDL_SCANCODE_UP;
	gameKeys[GAMEKEY_LEFT] = SDL_SCANCODE_LEFT;
	gameKeys[GAMEKEY_RIGHT] = SDL_SCANCODE_RIGHT;
	gameKeys[GAMEKEY_DOWN] = SDL_SCANCODE_DOWN;
	gameKeys[GAMEKEY_A] = SDL_SCANCODE_Z;
	gameKeys[GAMEKEY_B] = SDL_SCANCODE_X;
	gameKeys[GAMEKEY_X] = SDL_SCANCODE_C;
	gameKeys[GAMEKEY_Y] = SDL_SCANCODE_V;
	gameKeys[GAMEKEY_START] = SDL_SCANCODE_RETURN;
	gameKeys[GAMEKEY_SELECT] = SDL_SCANCODE_BACKSPACE;
	gameKeys[GAMEKEY_TRIGGER_L] = SDL_SCANCODE_Q;
	gameKeys[GAMEKEY_TRIGGER_R] = SDL_SCANCODE_W;

	screenManager.SetGameKeyBindings(gameKeys, GAMEKEY_MAX);
}

int main(int argc, char **argv)
{
	std::cout << "main()" << std::endl;
	std::cout << "Initializing SDL... " << std::endl;
	if (!initializeSDL())
	{
		terminateSDL();
		return 1;
	}
	std::cout << "SDL is init!!" << std::endl;

	std::cout << "Initializing ScreenManager... ";
	if (!screenManager.Init())
	{
		std::cout << "FAILED" << std::endl;
		terminateSDL();
		return 1;
	}
	std::cout << "OK" << std::endl;

	screenManager.SetRenderer(ren);
	std::cout << "Loading resources... ";
	if (!screenManager.LoadGlobalResources())
	{
		std::cout << "FAILED" << std::endl;
		std::cout << screenManager.GetLastError() << std::endl;
		terminateSDL();
		return 1;
	}
	std::cout << "OK" << std::endl;

	setKeyBindings();

	std::cout << "APP IS START" << std::endl;

	// TEMP TEMP TEMP!
	screenManager.GetResourceManager()->LoadImage("SnowCloseUp", "data/wallpapers/SnowCloseUp.jpg");
	screenManager.GetResourceManager()->LoadImage("GothenburgNight", "data/wallpapers/GothenburgNight.jpg");
	screenManager.GetResourceManager()->LoadImage("Bug", "data/wallpapers/Bug.jpg");
	screenManager.GetResourceManager()->LoadImage("Circuit", "data/wallpapers/Circuit.jpg");
	screenManager.GetResourceManager()->LoadImage("Leaves", "data/wallpapers/Leaves.jpg");

	screenManager.GetResourceManager()->LoadImage("IconApplications", "data/graphics/icon_applications.png");
	screenManager.GetResourceManager()->LoadImage("IconEmulators", "data/graphics/icon_emulators.png");
	screenManager.GetResourceManager()->LoadImage("IconGames", "data/graphics/icon_games.png");
	screenManager.GetResourceManager()->LoadImage("IconSettings", "data/graphics/icon_settings.png");
	
	ScreenBackgroundImage* bgScreen = new ScreenBackgroundImage();
	screenManager.AddScreen(bgScreen);
	screenManager.AddScreen(new ScreenMenu());
	//screenManager.AddScreen(new ScreenTest());

	bgScreen->AddImage("Circuit");
	bgScreen->AddImage("Bug");
	bgScreen->AddImage("SnowCloseUp");
	bgScreen->AddImage("GothenburgNight");
	bgScreen->AddImage("Leaves");

	int currentTime = SDL_GetTicks();
	double accumulator = 0;
	double frameTime = 1000.0/(double)FPS;
	int newTime;
	int deltaTime;
	int shouldDraw = 0;

	while(!screenManager.HasExit())
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
			_resetFrameSkip = 0;
		}

		while (accumulator >= frameTime)
		{
			accumulator -= frameTime;

			screenManager.Update();
			shouldDraw = 1;
		}

		if (shouldDraw)
		{
			screenManager.Draw();
			shouldDraw = 0;
		}
		else
		{
			SDL_Delay(1);
		}
	}
	
	terminateSDL();
	return 0;
}
