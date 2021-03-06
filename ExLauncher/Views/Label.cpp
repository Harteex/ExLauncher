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

#include "Label.h"
#include <SDL.h>
#include "../Graphics/graphics_utils.h"
#include <sstream>
#include <limits.h>
#include "../ScreenSystem/Screen.h"
#include "../ViewSystem/LayoutHelper.h"
#include "../Graphics/GaussianBlur.h"
#include <algorithm>

using namespace std;

Label::Label()
{
	texture = NULL;
	textureBg = NULL;
	text = "";
	textSize = 12;
	textColor = Color(0xff, 0xff, 0xff, 0xff);
	font = "semibold";
	textStyle = TextStylePlain;
	letterCase = LetterCaseOriginal;
	ttfFont = NULL;
}

Label::~Label()
{
	if (texture != NULL)
		SDL_DestroyTexture(texture);

	if (textureBg != NULL)
		SDL_DestroyTexture(texture);
}

void Label::OnInitialize()
{
	ttfFont = context->GetResourceManager()->GetTTFFont(font, textSize);

	if (ttfFont == NULL)
		throw runtime_error("Could not find specified font.");

	if (!RenderText(UINT_MAX))
		throw runtime_error("Text rendering failed.");
}

string Label::ToUpper(string s)
{
	transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
		{
			return toupper(c);
		}
	);

	return s;
}

string Label::ToLower(string s)
{
	transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
		{
			return tolower(c);
		}
	);

	return s;
}

bool Label::RenderText(Uint32 textAreaWidth)
{
	if (ttfFont == NULL)
		return false;

	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
	}

	if (textureBg != NULL)
	{
		SDL_DestroyTexture(textureBg);
		textureBg = NULL;
	}

	if (text == "")
	{
		contentSize.w = 0;
		contentSize.h = 0;
		return true;
	}

	string finalText = text;
	if (letterCase == LetterCaseUpper)
		finalText = ToUpper(text);
	else if (letterCase == LetterCaseLower)
		finalText = ToLower(text);

	int outlineSize = 1;

	int padding = 0;
	if (textStyle == TextStyleShadow)
		padding = outlineSize;

	SDL_Color white = { 0xFF, 0xFF, 0xFF };
	SDL_Color black = { 0x00, 0x00, 0x00 };

	SDL_Surface* tempSurface = NULL;
	if (textAreaWidth < UINT_MAX)
		tempSurface = TTF_RenderText_Blended_Wrapped(ttfFont, finalText.c_str(), white, textAreaWidth - padding * 2);
	else
		tempSurface = TTF_RenderText_Blended(ttfFont, finalText.c_str(), white);

	if (tempSurface == NULL)
		return false;

	SDL_Surface* tempSurfaceBg = NULL;
	if (textStyle == TextStyleShadow)
	{
		auto extendedSurface = context->GetGraphics().ExtendSurface(tempSurface, padding, padding);
		SDL_FreeSurface(tempSurface);
		tempSurface = extendedSurface;

		TTF_SetFontOutline(ttfFont, outlineSize);

		if (textAreaWidth < UINT_MAX)
			tempSurfaceBg = TTF_RenderText_Blended_Wrapped(ttfFont, finalText.c_str(), black, textAreaWidth - padding * 2);
		else
			tempSurfaceBg = TTF_RenderText_Blended(ttfFont, finalText.c_str(), black);

		TTF_SetFontOutline(ttfFont, 0);

		GaussianBlur blur(tempSurfaceBg);
		SDL_FreeSurface(tempSurfaceBg);
		tempSurfaceBg = blur.ProcessAlphaOnly(outlineSize);
	}

	SDL_Surface* clippedSurface = NULL;
	SDL_Surface* clippedSurfaceBg = NULL;

	int areaWidth = size.w;
	if (size.w == SIZE_FILL_PARENT)
		areaWidth = calculatedSize.w;
	else if (size.w == SIZE_WRAP_CONTENT)
		areaWidth = tempSurface->w;

	if (areaWidth > 0 && tempSurface->w > areaWidth)
	{
		SDL_Rect r;
		r.x = 0;
		r.y = 0;
		r.w = areaWidth;
		r.h = tempSurface->h;
		clippedSurface = context->GetGraphics().ClipSurface(tempSurface, &r);
		if (textStyle == TextStyleShadow)
			clippedSurfaceBg = context->GetGraphics().ClipSurface(tempSurfaceBg, &r);

		SDL_FreeSurface(tempSurface);
		if (textStyle == TextStyleShadow)
			SDL_FreeSurface(tempSurfaceBg);

		// Apply alpha fadeout at end if text doesn't fit completely
		context->GetGraphics().FadeOutSurface(clippedSurface, 16);
		if (textStyle == TextStyleShadow)
			context->GetGraphics().FadeOutSurface(clippedSurfaceBg, 16);
	}
	else
	{
		clippedSurface = tempSurface;
		clippedSurfaceBg = tempSurfaceBg;
	}

	if (clippedSurface == NULL)
		return false;

	texture = context->GetGraphics().CreateTextureFromSurface(clippedSurface);
	SDL_FreeSurface(clippedSurface);
	if (texture == NULL)
		return false;

	if (textStyle == TextStyleShadow)
	{
		SDL_SetSurfaceAlphaMod(clippedSurfaceBg, 0xb0);
		textureBg = context->GetGraphics().CreateTextureFromSurface(clippedSurfaceBg);
		SDL_FreeSurface(clippedSurfaceBg);

		if (textureBg == NULL)
			return false;
	}

	SDL_QueryTexture(texture, NULL, NULL, &contentSize.w, &contentSize.h);

	return true;
}

void Label::OnDraw(Graphics& graphics, Position offset)
{
	if (texture == NULL)
		return;

	SDL_Rect r;
	r.x = absolutePosition.x + offset.x;
	r.y = absolutePosition.y + offset.y;
	r.w = contentSize.w;
	r.h = contentSize.h;

	Position gravityOffset = LayoutHelper::GetGravityOffset(contentSize, calculatedSize, gravity);
	r.x += gravityOffset.x;
	r.y += gravityOffset.y;

	SDL_SetTextureColorMod(texture, textColor.r, textColor.g, textColor.b);

	if (textureBg != NULL)
		graphics.DrawTexture(&r, textureBg);

	graphics.DrawTexture(&r, texture);
}

void Label::OnLayoutChange()
{
	if (size.w == SIZE_FILL_PARENT || size.h == SIZE_FILL_PARENT)
		RenderText(UINT_MAX);
}

View* Label::Copy()
{
	Label* view = new Label();

	CopyBase(view);
	view->SetText(text);
	view->SetTextSize(textSize);
	view->SetTextColor(textColor);
	view->SetFont(font);
	view->SetTextStyle(textStyle);
	view->SetLetterCase(letterCase);

	return view;
}

string Label::GetText()
{
	return text;
}

void Label::SetText(string text)
{
	this->text = text;

	if (IsInitialized())
	{
		RenderText(UINT_MAX);

		if (size.w == SIZE_WRAP_CONTENT || size.h == SIZE_WRAP_CONTENT)
		{
			View* parent = GetParentView();
			if (parent != NULL)
				parent->RecalculateLayout();
		}
	}
}

int Label::GetTextSize()
{
	return textSize;
}

void Label::SetTextSize(int textSize)
{
	this->textSize = textSize;
}

Color Label::GetTextColor()
{
	return textColor;
}

void Label::SetTextColor(Color textColor)
{
	this->textColor = textColor;
}

string Label::GetFont()
{
	return font;
}

void Label::SetFont(string font)
{
	this->font = font;
}

TextStyle Label::GetTextStyle()
{
	return textStyle;
}

void Label::SetTextStyle(TextStyle textStyle)
{
	this->textStyle = textStyle;
}

LetterCase Label::GetLetterCase()
{
	return letterCase;
}

void Label::SetLetterCase(LetterCase letterCase)
{
	this->letterCase = letterCase;
}

bool Label::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	if (name == "text")
	{
		SetText(value);
		return true;
	}
	else if (name == "textSize")
	{
		textSize = atoi(value.c_str());

		if (textSize <= 0)
			throw runtime_error("invalid textSize value");

		return true;
	}
	else if (name == "textColor")
	{
		this->textColor = ParseColor(value);
		return true;
	}
	else if (name == "font")
	{
		font = value;
		return true;
	}
	else if (name == "textStyle")
	{
		if (value == "plain")
			SetTextStyle(TextStylePlain);
		else if (value == "shadow")
			SetTextStyle(TextStyleShadow);
		else
			throw runtime_error("invalid textStyle value");

		return true;
	}
	else if (name == "letterCase")
	{
		if (value == "original")
			SetLetterCase(LetterCaseOriginal);
		else if (value == "upper")
			SetLetterCase(LetterCaseUpper);
		else if (value == "lower")
			SetLetterCase(LetterCaseLower);
		else
			throw runtime_error("invalid letterCase value");

		return true;
	}

	return false;
}

void Label::FillData(map<string, string>& data)
{
	View::FillData(data);

	text = FindAndReplace(text, data);

	if (IsInitialized())
	{
		RenderText(UINT_MAX);
		RecalculateLayout();
	}
}
