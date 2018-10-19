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

#ifndef _LABEL_H_
#define _LABEL_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include <vector>
#include <string>
#include "../graphics_utils.h"
#include <SDL_ttf.h>

/*********************************************/

class Label : public View
{
private:
	SDL_Texture* texture;
	SDL_Texture* textureBg;
	std::string text;
	int textSize;
	Color textColor;
	std::string font;
	TextStyle textStyle;
	ResourceManager* resourceManager;
	TTF_Font* ttfFont;

	bool RenderText(Uint32 textAreaWidth);
public:
	Label();
	~Label();
	void OnInitialize();
	void OnDraw(Graphics& graphics, Position offset);
	void OnLayoutChange();
	View* Copy();

	std::string GetText();
	void SetText(std::string text);
	int GetTextSize();
	void SetTextSize(int textSize);
	Color GetTextColor();
	void SetTextColor(Color textColor);
	std::string GetFont();
	void SetFont(std::string font);
	TextStyle GetTextStyle();
	void SetTextStyle(TextStyle textStyle);
	bool SetProperty(std::string name, std::string value);
	void FillData(std::map<std::string, std::string>& data);
};

/*********************************************/

#endif
