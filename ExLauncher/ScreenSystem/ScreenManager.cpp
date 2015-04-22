#include "ScreenManager.h"
#include "Screen.h"
#include "../Screens/ScreenError.h"

using namespace std;

ScreenManager::ScreenManager()
{
	input = NULL;
	renderer = NULL;
	displaySize.w = 0;
	displaySize.h = 0;
	lastError = "No error";
}

ScreenManager::~ScreenManager()
{
	if (input != NULL)
		delete input;
}

bool ScreenManager::Init()
{
	input = new InputState();

	if (!appManager.LoadApps())
		return false;

	return true;
}

bool ScreenManager::LoadGlobalResources()
{
	//if (GetResourceManager()->LoadTTFFont("guiFont", "data/fonts/Orbitron Medium.ttf", 14) == NULL)
	if (GetResourceManager()->LoadTTFFont("guiFont", "data/fonts/OpenSans-Semibold.ttf", 18) == NULL)
	//if (GetResourceManager()->LoadTTFFont("guiFont", "data/fonts/LeagueGothic-Regular.otf", 18) == NULL)
	{
		lastError = "Failed to load data/fonts/OpenSans-Semibold.ttf";
		return false;
	}

	if (GetResourceManager()->LoadTTFFont("guiFontSmall", "data/fonts/OpenSans-Semibold.ttf", 12) == NULL)
	//if (GetResourceManager()->LoadTTFFont("guiFont", "data/fonts/LeagueGothic-Regular.otf", 18) == NULL)
	{
		lastError = "Failed to load data/fonts/OpenSans-Semibold.ttf";
		return false;
	}

	/*if (GetResourceManager()->CreateFont("dialogFont", GetResourceManager()->GetTTFFont("guiFont")) == NULL)
	{
		lastError = "Failed to build bitmap font";
		return false;
	}

	if (GetResourceManager()->CreateFont("dialogFontSmall", GetResourceManager()->GetTTFFont("guiFontSmall")) == NULL)
	{
		lastError = "Failed to build bitmap font";
		return false;
	}*/

	return true;
}

void ScreenManager::SetRenderer(SDL_Renderer* renderer)
{
	this->renderer = renderer;
	resourceManager.SetRenderer(renderer);

	if (renderer != NULL)
	{
		SDL_Rect r;
		SDL_RenderGetViewport(renderer, &r);
		displaySize.w = r.w;
		displaySize.h = r.h;
	}
}

SDL_Renderer* ScreenManager::GetRenderer()
{
	return renderer;
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

void ScreenManager::Draw()
{
	if (renderer == NULL)
		return;

	if (HasExit())
		return;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	for (list<Screen *>::iterator i = screens.end(); i != screens.begin(); )
	{
		i--;

		if ((*i)->GetScreenState() == Hidden || (*i)->HasExited())
			continue;

		(*i)->DrawHelper(renderer);
	}

	SDL_RenderPresent(renderer);
}

void ScreenManager::Update()
{
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

	if (!screen->Initialize())
	{
		string errMsg = "Failed to initialize screen: ";
		errMsg = errMsg + screen->GetLastError();
		delete screen;

		std::cout << errMsg << std::endl;

		ScreenError* errorScreen = new ScreenError(errMsg);
		errorScreen->SetScreenManager(this);
		if (!errorScreen->Initialize())
			Exit();

		screens.push_front(errorScreen);
	}
	else
	{
		screens.push_front(screen);
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
