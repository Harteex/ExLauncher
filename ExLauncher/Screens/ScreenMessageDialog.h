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

#ifndef _SCREEN_MESSAGE_DIALOG_H_
#define _SCREEN_MESSAGE_DIALOG_H_

/*********************************************/

#include "../ScreenSystem/Screen.h"
#include "../ScreenSystem/ScreenManager.h"

/*********************************************/

class ScreenMessageDialog : public Screen
{
private:
	Size dialogSize;
	int dialogPadding;
	SDL_Texture* textImage;
	bool textImageCreated;
	std::string text;
public:
	ScreenMessageDialog();
	~ScreenMessageDialog();
	void HandleInput(InputState* input);
	bool Initialize(Graphics& graphics);
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
	void SetText(std::string text);
protected:
	bool BuildTextImage(Graphics& graphics);
	void Draw(Graphics& graphics);
};

/*********************************************/

#endif
