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

	if (icon != NULL)
		SDL_DestroyTexture(icon);
}

bool ScreenProgressDialog::Initialize()
{
	if (text.length() == 0)
		return true;

	icon = loadImage("data/graphics/iconLoading.png", screenManager->GetRenderer());
	if (icon == NULL)
		return false;

	return BuildTextImage();
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

bool ScreenProgressDialog::BuildTextImage()
{
	if (textImage != NULL)
	{
		SDL_DestroyTexture(textImage);
		textImage = NULL;
	}

	TTF_Font* font = screenManager->GetResourceManager()->GetTTFFont("semibold", 12);
	SDL_Renderer* renderer = screenManager->GetRenderer();

	if (font == NULL || renderer == NULL)
		return false;

	SDL_Color fontColor;
	fontColor.r = 0xff;
	fontColor.g = 0xff;
	fontColor.b = 0xff;
	fontColor.a = 0xff;
	SDL_Surface* surfaceText = TTF_RenderText_Blended(font, text.c_str(), fontColor);
	if (surfaceText == NULL)
		return false;

	textImage = SDL_CreateTextureFromSurface(renderer, surfaceText);
	SDL_FreeSurface(surfaceText);
	if (textImage == NULL)
		return false;

	return true;
}

void ScreenProgressDialog::Draw(SDL_Renderer* renderer)
{
	int dialogX = (320 - dialogSize.w) / 2;
	int dialogY = (240 - dialogSize.h) / 2;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xA0 * (1 - transitionPosition));
	SDL_RenderFillRect(renderer, NULL);

	if (transitionPosition < 0.5)
	{
		SDL_Rect r;
		r.x = dialogX;
		r.y = dialogY;
		r.w = dialogSize.w;
		r.h = dialogSize.h;

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
		SDL_RenderFillRect(renderer, &r);

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

		if (textImage == NULL && text.length() > 0)
			BuildTextImage();

		if (textImage != NULL)
		{
			drawTexture(dialogX + 6, dialogY + dialogSize.h / 2, textImage, renderer, VCENTER|LEFT);
		}

		if (icon != NULL)
		{
			drawTexture(dialogX + dialogSize.w - 25, dialogY + dialogSize.h / 2, icon, renderer, VCENTER|HCENTER, NULL, iconRotation, 1.0, false);
		}
	}
	else
	{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	}
}
