#include "ScreenBackgroundImage.h"
#include "../global.h"

using namespace std;

// TODO:
// * method for setting cur image no
// * method to set random image no

ScreenBackgroundImage::ScreenBackgroundImage()
{
	curImage = 0;
	curTime = 0;
	isInTransition = false;

	SetTimeBetweenImages(20);
	SetTransitionTime(1);
}

bool ScreenBackgroundImage::Initialize()
{
	return true;
}

void ScreenBackgroundImage::SetTimeBetweenImages(int seconds)
{
	timeBetweenImages = seconds * FPS;
}

void ScreenBackgroundImage::SetTransitionTime(int seconds)
{
	transitionTime = seconds * FPS;
}

void ScreenBackgroundImage::HandleInput(InputState* input)
{
	//
}

void ScreenBackgroundImage::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	// Always report not covered by other screen since it should be covered and still visible.
	// This prevents the base screen from hiding it.
	Screen::Update(otherScreenHasFocus, false);

	if (images.size() <= 1)
		return;

	curTime++;
	if (curTime == timeBetweenImages)
	{
		curTime = 0;
		curImage = (curImage + 1) % images.size();
		if (transitionTime > 0)
			isInTransition = true;
	}

	if (curTime > transitionTime)
		isInTransition = false;
}

void ScreenBackgroundImage::AddImage(string id)
{
	SDL_Texture* img = screenManager->GetResourceManager()->GetImage(id);
	if (img != NULL)
		images.push_back(img);
}

void ScreenBackgroundImage::Draw(SDL_Renderer* renderer)
{
	if (images.empty())
	{
		// FIXME draw black
		return;
	}

	if (isInTransition)
	{
		// Draw old image
		int prevImage = curImage - 1;
		if (prevImage < 0)
			prevImage += images.size();

		drawTexture(0, 0, images.at(prevImage), renderer, TOP|LEFT);

		// Draw new with appropriate alpha
		SDL_Texture* curTexture = images.at(curImage);
		SDL_SetTextureAlphaMod(curTexture, 0xff * ((double)curTime / (double)transitionTime));
		drawTexture(0, 0, curTexture, renderer, TOP|LEFT);
	}
	else
	{
		SDL_Texture* curTexture = images.at(curImage);
		SDL_SetTextureAlphaMod(curTexture, 0xff);
		drawTexture(0, 0, curTexture, renderer, TOP|LEFT);
	}
}
