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
