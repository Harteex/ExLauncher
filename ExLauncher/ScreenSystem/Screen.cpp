#include "Screen.h"
#include "../utils.h"
#include "../global.h"

using namespace std;

Screen::Screen()
{
	screenstate = Active;
	lastError = "No error.";
	isExiting = false;
	exited = false;
	otherScreenHasFocus = true;
	popup = false;
	screenManager = NULL;
	transitionPosition = 1;
	transitionOnTime = 0.4;
	transitionOffTime = 0.4;
}

Screen::~Screen()
{
}

ScreenState Screen::GetScreenState()
{
	return screenstate;
}

string Screen::GetLastError()
{
	return lastError;
}

void Screen::DrawHelper(SDL_Renderer* renderer)
{
	/*if (SDL_MUSTLOCK(display))
    {
        if (SDL_LockSurface(display) < 0)
			return;
    }*/

	Draw(renderer);

	/*if (SDL_MUSTLOCK(display))
		SDL_UnlockSurface(display);*/
}

void Screen::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	this->otherScreenHasFocus = otherScreenHasFocus;

	if (isExiting)
	{
		screenstate = TransitionOff;

		if (!UpdateTransition(transitionOffTime, 1))
		{
			exited = true;
		}
	}
	else if (coveredByOtherScreen)
	{
		if (UpdateTransition(transitionOffTime, 1))
		{
			screenstate = TransitionOff;
		}
		else
		{
			screenstate = Hidden;
		}
	}
	else
	{
		if (UpdateTransition(transitionOnTime, -1))
		{
			screenstate = TransitionOn;
		}
		else
		{
			screenstate = Active;
		}
	}
}

bool Screen::UpdateTransition(double time, int direction)
{
	double transitionDelta;
	if (time == 0)
		transitionDelta = 1;
	else
		transitionDelta = (1.0 / (double)FPS) / time;

	transitionPosition += transitionDelta * direction;
	if (transitionPosition <= 0 && direction < 0 || transitionPosition >= 1 && direction > 0)
	{
		transitionPosition = clip(transitionPosition, 0.0, 1.0);
		return false;
	}

	return true;
}

void Screen::ExitScreen()
{
	isExiting = true;
}

bool Screen::IsExiting()
{
	return isExiting;
}

bool Screen::HasExited()
{
	return exited;
}

bool Screen::IsPopup()
{
	return popup;
}

bool Screen::IsActive()
{
	return !otherScreenHasFocus && (screenstate == TransitionOn || screenstate == Active);
}

void Screen::SetScreenManager(ScreenManager* screenManager)
{
	this->screenManager = screenManager;
}

double Screen::GetTransitionPosition()
{
	return transitionPosition;
}

Uint8 Screen::GetTransitionAlpha()
{
	return (Uint8)((1 - transitionPosition) * 255);
}

void Screen::SetTransitionTime(double transitionOnTime, double transitionOffTime)
{
	this->transitionOnTime = transitionOnTime;
	this->transitionOffTime = transitionOffTime;

	if (transitionOnTime == 0)
		transitionPosition = 0;
}
