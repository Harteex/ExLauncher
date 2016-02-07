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

#ifndef _SCREEN_APP_LAUNCH_H_
#define _SCREEN_APP_LAUNCH_H_

/*********************************************/

#include "../ScreenSystem/Screen.h"
#include "../ScreenSystem/ScreenManager.h"
#include "../structures.h"

/*********************************************/

class ScreenAppLaunch : public Screen
{
private:
	Box origBox;
	Box curBox;
	std::string appId;
	std::vector<std::string> exec;
public:
	ScreenAppLaunch();
	~ScreenAppLaunch();
	void SetStartRectangle(int x, int y, int width, int height);
	void SetAppId(std::string appId);
	void SetExec(std::vector<std::string> exec);
	void HandleInput(InputState* input);
	bool Initialize();
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
protected:
	void Draw(SDL_Renderer* renderer);
};

/*********************************************/

#endif
