#include "ScreenTest.h"
#include "../graphics.h"

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
	if (texture != NULL)
		SDL_DestroyTexture(texture);

	if (icon1 != NULL)
		SDL_DestroyTexture(icon1);

	if (icon2 != NULL)
		SDL_DestroyTexture(icon2);

	if (icon3 != NULL)
		SDL_DestroyTexture(icon3);
}

bool ScreenTest::Initialize()
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

	SDL_Surface* surface = CreateGradientSurface(50, 50, fromColor, toColor);
	if (surface == NULL)
		return false;

	//dumpSurface("surf.bin", surface);

	texture = SDL_CreateTextureFromSurface(screenManager->GetRenderer(), surface);
	SDL_FreeSurface(surface);

	if (texture == NULL)
		return false;

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	icon1 = loadImage("data/graphics/testicon1.png", screenManager->GetRenderer());
	icon2 = loadImage("data/graphics/testicon2.png", screenManager->GetRenderer());
	icon3 = loadImage("data/graphics/testicon3.png", screenManager->GetRenderer());

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

void ScreenTest::Draw(SDL_Renderer* renderer)
{
	if (texture != NULL)
	{
		drawTexture(5, 5, texture, renderer, TOP|LEFT);
	}

	drawTexture(40, 50, icon1, renderer, VCENTER|HCENTER, NULL, 0, 0.9, false);
	drawTexture(80, 50, icon1, renderer, VCENTER|HCENTER, NULL, 0, 1, false);
	drawTexture(120, 50, icon1, renderer, VCENTER|HCENTER, NULL, 0, 1.1, false);

	drawTexture(40, 100, icon2, renderer, VCENTER|HCENTER, NULL, 0, 0.9, false);
	drawTexture(80, 100, icon2, renderer, VCENTER|HCENTER, NULL, 0, 1, false);
	drawTexture(120, 100, icon2, renderer, VCENTER|HCENTER, NULL, 0, 1.1, false);

	drawTexture(40, 150, icon3, renderer, VCENTER|HCENTER, NULL, 0, 0.9, false);
	drawTexture(80, 150, icon3, renderer, VCENTER|HCENTER, NULL, 0, 1, false);
	drawTexture(120, 150, icon3, renderer, VCENTER|HCENTER, NULL, 0, 1.1, false);
}

void ScreenTest::SetError(string errorMsg)
{
	//
}
