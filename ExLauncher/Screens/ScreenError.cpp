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

#include "ScreenError.h"

using namespace std;

ScreenError::ScreenError(string errorText)
{
	this->errorLabel = NULL;
	this->errorText = errorText;
}

bool ScreenError::Initialize()
{
	errorLabel = new Label();

	if (errorText == "")
		errorText = "No error";

	errorLabel->SetText(errorText);

	if (!errorLabel->Initialize(this))
		return false;

	errorLabel->SetSize(SIZE_FILL_PARENT, SIZE_WRAP_CONTENT);
	errorLabel->SetRelativePosition(0, 0);
	errorLabel->CalculateLayout(screenManager->GetDisplaySize());
	errorLabel->CalculateAbsolutePosition(Position(0, 0));

	return true;
}

void ScreenError::HandleInput(InputState* input)
{
	// Check for any key, and exit
}

void ScreenError::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);
}

void ScreenError::Draw(SDL_Renderer* renderer)
{
	if (errorLabel != NULL)
	{
		errorLabel->Draw();
		// Print error on screen using the dialog font
		//st_font_draw_text_ascii_center(errorMsg.c_str(), 50, screenManager->GetResourceManager()->GetFont("dialogFont"), renderer); // FIXME
	}
}
