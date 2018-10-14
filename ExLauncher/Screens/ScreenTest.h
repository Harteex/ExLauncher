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

#ifndef _SCREEN_TEST_H_
#define _SCREEN_TEST_H_

/*********************************************/

#include "../ScreenSystem/Screen.h"
#include "../ScreenSystem/ScreenManager.h"

/*********************************************/

class ScreenTest : public Screen
{
private:
	SDL_Texture* texture;
	SDL_Texture* texture2;
	SDL_Texture* texture3;
	SDL_Texture* icon1;
	SDL_Texture* icon2;
	SDL_Texture* icon3;
	TTF_Font* font;
	TTF_Font* font_outline;
	SDL_Texture* texttexture;
	SDL_Texture* texttexture2;
public:
	ScreenTest();
	~ScreenTest();
	void HandleInput(InputState* input);
	bool Initialize(Graphics& graphics);
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
	void SetError(std::string errorMsg);
protected:
	void Draw(Graphics& graphics);
};

/*********************************************/

#endif
