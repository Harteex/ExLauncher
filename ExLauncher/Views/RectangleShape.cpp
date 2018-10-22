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

#include "RectangleShape.h"
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

void RectangleShape::OnInitialize()
{	
}

void RectangleShape::OnDraw(Graphics& graphics, Position offset)
{
	if (strokeColor.a == 0 && fillColor.a == 0)
		return;

	Rectangle r;
	r.x = absolutePosition.x + offset.x;
	r.y = absolutePosition.y + offset.y;
	r.w = calculatedSize.w;
	r.h = calculatedSize.h;

	if (fillColor.a > 0)
	{
		graphics.FillRect(r, fillColor);
	}

	if (strokeColor.a > 0)
	{
		graphics.DrawRect(r, strokeWidth, strokeColor);
	}
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
