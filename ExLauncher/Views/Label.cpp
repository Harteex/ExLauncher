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
#include "../graphics_utils.h"
#include <sstream>
#include <limits.h>
#include "../ScreenSystem/Screen.h"
#include "../ViewSystem/LayoutHelper.h"

using namespace std;

Label::Label()
{
	texture = NULL;
	text = "";
	textSize = 12;
	textColor = Color(0xff, 0xff, 0xff, 0xff);
	font = "semibold";
	ttfFont = NULL;
}

Label::~Label()
{
	if (texture != NULL)
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

bool Label::RenderText(Uint32 textAreaWidth)
{
	if (ttfFont == NULL)
		return false;

	if (texture != NULL)
		SDL_DestroyTexture(texture);

	texture = NULL;

	SDL_Color color;
	color.r = 0xff;
	color.g = 0xff;
	color.b = 0xff;
	color.a = 0xff;
	SDL_Surface* tempSurface = TTF_RenderText_Blended_Wrapped(ttfFont, text.c_str(), color, textAreaWidth);
	if (tempSurface == NULL)
		return false;

	SDL_Surface* clippedSurface = NULL;

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
		clippedSurface = ClipSurface(tempSurface, &r);

		SDL_FreeSurface(tempSurface);

		// Apply alpha fadeout at end if text doesn't fit completely
		FadeOutSurface(clippedSurface, 16);
	}
	else
	{
		clippedSurface = tempSurface;
	}

	if (clippedSurface == NULL)
		return false;

	texture = SDL_CreateTextureFromSurface(context->GetRenderer(), clippedSurface);
	SDL_FreeSurface(clippedSurface);
	if (texture == NULL)
		return false;

	SDL_QueryTexture(texture, NULL, NULL, &contentSize.w, &contentSize.h);

	return true;
}

void Label::OnDraw(SDL_Renderer* renderer, Position offset)
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

	drawTexture(&r, texture, renderer);
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
