#include "RectangleShape.h"
#include "../ThemeManager.h"
#include "../ScreenSystem/Screen.h"
#include <sstream>

using namespace std;

RectangleShape::RectangleShape()
{
	strokeColor = Color(0);
	fillColor = Color(0);
	strokeWidth = 1;
}

RectangleShape::~RectangleShape()
{
}

bool RectangleShape::OnInitialize()
{	
	return true;
}

void RectangleShape::OnDraw(SDL_Renderer* renderer, Position offset)
{
	if (strokeColor.a == 0 && fillColor.a == 0)
		return;

	SDL_Rect r;
	r.x = absolutePosition.x + offset.x;
	r.y = absolutePosition.y + offset.y;
	r.w = calculatedSize.w;
	r.h = calculatedSize.h;

	if (fillColor.a > 0)
	{
		if (fillColor.a < 0xff)
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		else
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

		SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);

		SDL_RenderFillRect(renderer, &r);
	}

	if (strokeColor.a > 0)
	{
		if (strokeColor.a < 0xff)
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		else
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

		SDL_SetRenderDrawColor(renderer, strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);

		if (strokeWidth == 1)
		{
			SDL_RenderDrawRect(renderer, &r);
		}
		else if (strokeWidth > 1)
		{
			SDL_Rect sides[4];

			sides[0].x = r.x;
			sides[0].y = r.y + strokeWidth;
			sides[0].w = strokeWidth;
			sides[0].h = r.h - strokeWidth * 2;

			sides[1].x = r.x + r.w - strokeWidth;
			sides[1].y = r.y + strokeWidth;
			sides[1].w = strokeWidth;
			sides[1].h = r.h - strokeWidth * 2;

			sides[2].x = r.x;
			sides[2].y = r.y;
			sides[2].w = r.w;
			sides[2].h = strokeWidth;

			sides[3].x = r.x;
			sides[3].y = r.y + r.h - strokeWidth;
			sides[3].w = r.w;
			sides[3].h = strokeWidth;

			SDL_RenderFillRects(renderer, sides, 4);
		}
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

View* RectangleShape::Copy()
{
	RectangleShape* view = new RectangleShape();

	CopyBase(view);
	view->SetStrokeColor(strokeColor);
	view->SetFillColor(fillColor);
	view->SetStrokeWidth(strokeWidth);

	return view;
}

void RectangleShape::SetStrokeColor(Color strokeColor)
{
	this->strokeColor = strokeColor;
}

void RectangleShape::SetFillColor(Color fillColor)
{
	this->fillColor = fillColor;
}

void RectangleShape::SetStrokeWidth(int strokeWidth)
{
	this->strokeWidth = strokeWidth;
}

bool RectangleShape::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	if (name == "strokeColor")
	{
		this->strokeColor = ParseColor(value);
		return true;
	}
	else if (name == "fillColor")
	{
		this->fillColor = ParseColor(value);
		return true;
	}
	else if (name == "strokeWidth")
	{
		stringstream ss(value);
		if ((ss >> strokeWidth).fail() || !(ss >> std::ws).eof())
		{
			throw runtime_error("could not parse strokeWidth");
		}

		return true;
	}

	return false;
}
