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
	bool Initialize();
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
	void SetText(std::string text);
protected:
	bool BuildTextImage();
	void Draw(SDL_Renderer* renderer);
};

/*********************************************/

#endif
