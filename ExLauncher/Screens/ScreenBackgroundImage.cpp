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
	pinned = true;

	SetTimeBetweenImages(20);
	SetTransitionTime(1);
}

bool ScreenBackgroundImage::Initialize(Graphics& graphics)
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

void ScreenBackgroundImage::Draw(Graphics& graphics)
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

		graphics.DrawTexture(0, 0, images.at(prevImage), TOP|LEFT);

		// Draw new with appropriate alpha
		SDL_Texture* curTexture = images.at(curImage);
		SDL_SetTextureAlphaMod(curTexture, 0xff * ((double)curTime / (double)transitionTime));
		graphics.DrawTexture(0, 0, curTexture, TOP|LEFT);
	}
	else
	{
		SDL_Texture* curTexture = images.at(curImage);
		SDL_SetTextureAlphaMod(curTexture, 0xff);
		graphics.DrawTexture(0, 0, curTexture, TOP|LEFT);
	}
}
