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

#include "ScreenAppLaunch.h"
#include "../global.h"

using namespace std;

ScreenAppLaunch::ScreenAppLaunch()
{
	exec = vector<string>();
	popup = true;
	drawnFramesAfterTransition = 0;
	SetTransitionTime(0.2, 0.1);
}

ScreenAppLaunch::~ScreenAppLaunch()
{
}

void ScreenAppLaunch::SetStartRectangle(int x, int y, int width, int height)
{
	origBox = Box(y, y + height, x, x + width);
	curBox = origBox;
}

void ScreenAppLaunch::SetAppId(std::string appId, std::string withFile)
{
	this->appId = appId;
	this->withFile = withFile;
}

void ScreenAppLaunch::SetExec(vector<string> exec)
{
	this->exec = exec;
}

bool ScreenAppLaunch::Initialize(Graphics& graphics)
{
	return true;
}

void ScreenAppLaunch::HandleInput(InputState* input)
{
	//
}

void ScreenAppLaunch::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);

	Size dispSize = screenManager->GetDisplaySize();

	// FIXME use sigmoid curve for transition instead of linear, or possibly only sigmoid end
	// FIXME transition in with alpha as well

	Box dBox;
	dBox.top = origBox.top;
	dBox.bottom = dispSize.h - origBox.bottom;
	dBox.left = origBox.left;
	dBox.right = dispSize.w - origBox.right;

	curBox.top = dBox.top * transitionPosition;
	curBox.bottom = dispSize.h - dBox.bottom * transitionPosition;
	curBox.left = dBox.left * transitionPosition;
	curBox.right = dispSize.w - dBox.right * transitionPosition;

	// Make sure we have drawn a few extra times because of double / triple buffering.
	// If we don't do this, a launched game might use the other buffer and that will
	// cause an image of the unfinished transition to appear for a short while.
	if (drawnFramesAfterTransition >= 3)
	{
		if (appId.empty())
			screenManager->GetAppManager()->SetCommandToLaunch(exec);
		else
			screenManager->GetAppManager()->SetAppToLaunch(appId, withFile, exec);

		screenManager->Exit();
	}
}

void ScreenAppLaunch::Draw(Graphics& graphics)
{
	graphics.FillRect(curBox, Color(0, 0, 0));

	if (TransitionHasFinished())
		drawnFramesAfterTransition++;
}
