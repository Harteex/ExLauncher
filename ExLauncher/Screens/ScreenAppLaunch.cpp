#include "ScreenAppLaunch.h"
#include "../global.h"

using namespace std;

ScreenAppLaunch::ScreenAppLaunch()
{
	exec = "";
	popup = true;
	SetTransitionTime(0.2, 0.1);
}

ScreenAppLaunch::~ScreenAppLaunch()
{
}

void ScreenAppLaunch::SetStartRectangle(int x, int y, int width, int height)
{
	origBox = Box(y, y + height, x, x + width);
	curBox = origBox;
}

void ScreenAppLaunch::SetExec(std::string exec)
{
	this->exec = exec;
}

bool ScreenAppLaunch::Initialize()
{
	return true;
}

void ScreenAppLaunch::HandleInput(InputState* input)
{
	//
}

void ScreenAppLaunch::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);

	Size dispSize = screenManager->GetDisplaySize();

	// FIXME use sigmoid curve for transition instead of linear

	Box dBox;
	dBox.top = origBox.top;
	dBox.bottom = dispSize.h - origBox.bottom;
	dBox.left = origBox.left;
	dBox.right = dispSize.w - origBox.right;

	curBox.top = dBox.top * transitionPosition;
	curBox.bottom = dispSize.h - dBox.bottom * transitionPosition;
	curBox.left = dBox.left * transitionPosition;
	curBox.right = dispSize.w - dBox.right * transitionPosition;

	if (IsActive())
	{
		// TODO when transitioned in completely, prepare for launching
	}
}

void ScreenAppLaunch::Draw(SDL_Renderer* renderer)
{
	SDL_Rect r;
	r.x = curBox.left;
	r.y = curBox.top;
	r.w = curBox.right - curBox.left;
	r.h = curBox.bottom - curBox.top;

	SDL_RenderFillRect(renderer, &r);
}
