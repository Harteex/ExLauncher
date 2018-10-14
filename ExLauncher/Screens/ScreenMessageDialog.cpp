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

#include "ScreenMessageDialog.h"
#include "../global.h"

using namespace std;

ScreenMessageDialog::ScreenMessageDialog()
{
	popup = true;
	SetTransitionTime(0.5, 0.2);
	dialogSize = Size(220, 100);
	dialogPadding = 8;
	textImage = NULL;
	textImageCreated = false;
	text = "";
}

ScreenMessageDialog::~ScreenMessageDialog()
{
	if (textImage != NULL)
		SDL_DestroyTexture(textImage);
}

bool ScreenMessageDialog::Initialize(Graphics& graphics)
{
	if (text.length() == 0)
		return true;

	return BuildTextImage(graphics);
}

void ScreenMessageDialog::SetText(string text)
{
	this->text = text;
	textImage = NULL;
}

void ScreenMessageDialog::HandleInput(InputState* input)
{
	if (input->GameKeyJustPressed(GAMEKEY_A) ||
		input->GameKeyJustPressed(GAMEKEY_B) ||
		input->GameKeyJustPressed(GAMEKEY_START))
	{
		ExitScreen();
	}
}

void ScreenMessageDialog::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);

	if (IsActive())
	{
		//
	}
}

bool ScreenMessageDialog::BuildTextImage(Graphics& graphics)
{
	if (textImage != NULL)
	{
		SDL_DestroyTexture(textImage);
		textImage = NULL;
	}

	if (text == "")
		return true;

	TTF_Font* font = screenManager->GetResourceManager()->GetTTFFont("semibold", 12);

	if (font == NULL)
		return false;

	textImage = graphics.RenderText(text.c_str(), Color(0xff, 0xff, 0xff), font, dialogSize.w - dialogPadding * 2);

	if (textImage == NULL)
		return false;

	textImageCreated = true;
	SDL_QueryTexture(textImage, NULL, NULL, NULL, &dialogSize.h);
	dialogSize.h += dialogPadding * 2;
	return true;
}

void ScreenMessageDialog::Draw(Graphics& graphics)
{
	int dialogX = (320 - dialogSize.w) / 2;
	int dialogY = (240 - dialogSize.h) / 2;

	graphics.FillRect(Color(0x00, 0x00, 0x00, 0xA0 * (1 - transitionPosition)));

	if (transitionPosition < 0.5)
	{
		Rectangle r;
		r.x = dialogX;
		r.y = dialogY;
		r.w = dialogSize.w;
		r.h = dialogSize.h;

		graphics.FillRect(r, Color(0x00, 0x00, 0x00, 0xff));

		if (!textImageCreated && text.length() > 0)
			BuildTextImage(graphics);

		if (textImage != NULL)
			graphics.DrawTexture(dialogX + dialogPadding, dialogY + dialogPadding, textImage, TOP|LEFT);
	}
}
