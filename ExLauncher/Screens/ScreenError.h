#ifndef _SCREEN_ERROR_H_
#define _SCREEN_ERROR_H_

/*********************************************/

#include "../ScreenSystem/Screen.h"
#include "../ScreenSystem/ScreenManager.h"
#include "../Views/Label.h"

/*********************************************/

class ScreenError : public Screen
{
private:
	Label* errorLabel;
	std::string errorText;
public:
	ScreenError(std::string errorText);
	void HandleInput(InputState* input);
	bool Initialize();
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
protected:
	void Draw(SDL_Renderer* renderer);
};

/*********************************************/

#endif
