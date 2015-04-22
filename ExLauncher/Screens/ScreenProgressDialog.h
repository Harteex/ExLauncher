#ifndef _SCREEN_PROGRESS_DIALOG_H_
#define _SCREEN_PROGRESS_DIALOG_H_

/*********************************************/

#include "../ScreenSystem/Screen.h"
#include "../ScreenSystem/ScreenManager.h"

/*********************************************/

class ScreenProgressDialog : public Screen
{
private:
	Size dialogSize;
	SDL_Texture* textImage;
	std::string text;
	SDL_Texture* icon;
	double iconRotation;
public:
	ScreenProgressDialog();
	~ScreenProgressDialog();
	void HandleInput(InputState* input);
	bool Initialize();
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
	void SetText(std::string text);
protected:
	bool BuildTextImage();
	void Draw(SDL_Renderer* renderer);
};

/*********************************************/

#endif
