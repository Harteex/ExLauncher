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

bool ScreenMessageDialog::Initialize()
{
	if (text.length() == 0)
		return true;

	return BuildTextImage();
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

bool ScreenMessageDialog::BuildTextImage()
{
	if (textImage != NULL)
	{
		SDL_DestroyTexture(textImage);
		textImage = NULL;
	}

	if (text == "")
		return true;

	TTF_Font* font = screenManager->GetResourceManager()->GetTTFFont("semibold", 12);
	SDL_Renderer* renderer = screenManager->GetRenderer();

	if (font == NULL || renderer == NULL)
		return false;

	textImage = drawText(text.c_str(), 0xff, 0xff, 0xff, font, renderer, dialogSize.w - dialogPadding * 2);

	if (textImage == NULL)
		return false;

	textImageCreated = true;
	SDL_QueryTexture(textImage, NULL, NULL, NULL, &dialogSize.h);
	dialogSize.h += dialogPadding * 2;
	return true;
}

void ScreenMessageDialog::Draw(SDL_Renderer* renderer)
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

		if (!textImageCreated && text.length() > 0)
			BuildTextImage();

		if (textImage != NULL)
			drawTexture(dialogX + dialogPadding, dialogY + dialogPadding, textImage, renderer, TOP|LEFT);
	}
	else
	{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	}
}
