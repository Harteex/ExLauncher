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

#ifndef _RECTANGLE_SHAPE_H_
#define _RECTANGLE_SHAPE_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include <vector>
#include <string>
#include "../Graphics/graphics_utils.h"

/*********************************************/

class RectangleShape : public View
{
private:
	Color strokeColor;
	Color fillColor;
	int strokeWidth;
public:
	RectangleShape();
	~RectangleShape();
	void OnInitialize();
	void OnDraw(Graphics& graphics, Position offset);
	View* Copy();

	void SetStrokeColor(Color strokeColor);
	void SetFillColor(Color fillColor);
	void SetStrokeWidth(int strokeWidth);
	bool SetProperty(std::string name, std::string value);
};

/*********************************************/

#endif
