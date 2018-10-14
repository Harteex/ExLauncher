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

#include "ScreenTest.h"
#include "../graphics_utils.h"

using namespace std;

ScreenTest::ScreenTest()
{
	texture = NULL;
	icon1 = NULL;
	icon2 = NULL;
	icon3 = NULL;
}

ScreenTest::~ScreenTest()
{
	auto resourceManager = screenManager->GetResourceManager();
	resourceManager->UnloadImage("test_icon_1");
	resourceManager->UnloadImage("test_icon_2");
	resourceManager->UnloadImage("test_icon_3");
	
	if (texture != NULL)
		SDL_DestroyTexture(texture);

	if (texture2 != NULL)
		SDL_DestroyTexture(texture2);

	if (texture3 != NULL)
		SDL_DestroyTexture(texture3);
}

bool ScreenTest::Initialize(Graphics& graphics)
{
	SDL_Color fromColor;
	fromColor.r = 0xff;
	fromColor.g = 0xff;
	fromColor.b = 0xff;
	fromColor.a = 0x50;

	SDL_Color toColor;
	toColor.r = 0xff;
	toColor.g = 0xff;
	toColor.b = 0xff;
	toColor.a = 0x00;

	SDL_Surface* surface = graphics.CreateGradientSurface(50, 50, fromColor, toColor);
	if (surface == NULL)
		return false;

	//dumpSurface("surf.bin", surface);

	texture = graphics.CreateTextureFromSurface(surface);
	SDL_FreeSurface(surface);

	if (texture == NULL)
		return false;

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	auto resourceManager = screenManager->GetResourceManager();

	icon1 = resourceManager->LoadImage("test_icon_1", "data/graphics/testicon1.png");
	icon2 = resourceManager->LoadImage("test_icon_2", "data/graphics/testicon2.png");
	icon3 = resourceManager->LoadImage("test_icon_3", "data/graphics/testicon3.png");

	return true;
}

void ScreenTest::HandleInput(InputState* input)
{
	// Check for any key, and exit
}

void ScreenTest::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);
}

void ScreenTest::Draw(Graphics& graphics)
{
	graphics.FillRect(5, 5, 65, 30, Color(0xff, 0xff, 0xff));
	
	if (texture != NULL)
	{
		graphics.DrawTexture(5, 5, texture, TOP|LEFT);
	}

	if (texture2 != NULL)
	{
		graphics.DrawTexture(240, 190, texture2, TOP | LEFT);
	}

	if (texture3 != NULL)
	{
		graphics.DrawTexture(280, 190, texture3, TOP | LEFT);
	}

	graphics.DrawTexture(40, 50, icon1, VCENTER|HCENTER, NULL, 0, 0.9, false);
	graphics.DrawTexture(80, 50, icon1, VCENTER|HCENTER, NULL, 0, 1, false);
	graphics.DrawTexture(120, 50, icon1, VCENTER|HCENTER, NULL, 0, 1.1, false);

	graphics.DrawTexture(40, 100, icon2, VCENTER|HCENTER, NULL, 0, 0.9, false);
	graphics.DrawTexture(80, 100, icon2, VCENTER|HCENTER, NULL, 0, 1, false);
	graphics.DrawTexture(120, 100, icon2, VCENTER|HCENTER, NULL, 0, 1.1, false);

	graphics.DrawTexture(40, 150, icon3, VCENTER|HCENTER, NULL, 0, 0.9, false);
	graphics.DrawTexture(80, 150, icon3, VCENTER|HCENTER, NULL, 0, 1, false);
	graphics.DrawTexture(120, 150, icon3, VCENTER|HCENTER, NULL, 0, 1.1, false);

	graphics.DrawTexture(50, 5, texttexture, 0);
	graphics.DrawTexture(51, 6, texttexture2, 0);
}

void ScreenTest::SetError(string errorMsg)
{
	//
}
