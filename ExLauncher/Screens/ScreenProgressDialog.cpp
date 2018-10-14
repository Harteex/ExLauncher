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

#include "ScreenProgressDialog.h"
#include "../global.h"

using namespace std;

ScreenProgressDialog::ScreenProgressDialog()
{
	popup = true;
	SetTransitionTime(0.2, 0.1);
	dialogSize.w = 220;
	dialogSize.h = 100;
	textImage = NULL;
	text = "";
	icon = NULL;
	iconRotation = 0.0;
}

ScreenProgressDialog::~ScreenProgressDialog()
{
	if (textImage != NULL)
		SDL_DestroyTexture(textImage);

	auto resourceManager = screenManager->GetResourceManager();
	resourceManager->UnloadImage("progress_dialog_icon");
}

bool ScreenProgressDialog::Initialize(Graphics& graphics)
{
	if (text.length() == 0)
		return true;

	auto resourceManager = screenManager->GetResourceManager();

	icon = resourceManager->LoadImage("progress_dialog_icon", "data/graphics/iconLoading.png");
	if (icon == NULL)
		return false;

	return BuildTextImage(graphics);
}

void ScreenProgressDialog::SetText(string text)
{
	this->text = text;

	if (textImage != NULL)
	{
		SDL_DestroyTexture(textImage);
		textImage = NULL;
	}
}

void ScreenProgressDialog::HandleInput(InputState* input)
{
	//
}

void ScreenProgressDialog::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);

	iconRotation -= 5;
	if (iconRotation < 0)
		iconRotation += 360;

	if (IsActive())
	{
		//
	}
}

bool ScreenProgressDialog::BuildTextImage(Graphics& graphics)
{
	if (textImage != NULL)
	{
		SDL_DestroyTexture(textImage);
		textImage = NULL;
	}

	TTF_Font* font = screenManager->GetResourceManager()->GetTTFFont("semibold", 12);

	if (font == NULL)
		return false;

	SDL_Color fontColor;
	fontColor.r = 0xff;
	fontColor.g = 0xff;
	fontColor.b = 0xff;
	fontColor.a = 0xff;
	SDL_Surface* surfaceText = TTF_RenderText_Blended(font, text.c_str(), fontColor);
	if (surfaceText == NULL)
		return false;

	textImage = graphics.CreateTextureFromSurface(surfaceText);
	SDL_FreeSurface(surfaceText);
	if (textImage == NULL)
		return false;

	return true;
}

void ScreenProgressDialog::Draw(Graphics& graphics)
{
	int dialogX = (320 - dialogSize.w) / 2;
	int dialogY = (240 - dialogSize.h) / 2;

	graphics.FillRect(Color(0x00, 0x00, 0x00, 0xa0 * (1 - transitionPosition)));

	if (transitionPosition < 0.5)
	{
		graphics.FillRect(dialogX, dialogY, dialogSize.w, dialogSize.h, Color(BLACK));

		if (textImage == NULL && text.length() > 0)
			BuildTextImage(graphics);

		if (textImage != NULL)
		{
			graphics.DrawTexture(dialogX + 6, dialogY + dialogSize.h / 2, textImage, VCENTER|LEFT);
		}

		if (icon != NULL)
		{
			graphics.DrawTexture(dialogX + dialogSize.w - 25, dialogY + dialogSize.h / 2, icon, VCENTER|HCENTER, NULL, iconRotation, 1.0, false);
		}
	}
}
