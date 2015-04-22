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
	SDL_Texture* icon1;
	SDL_Texture* icon2;
	SDL_Texture* icon3;
public:
	ScreenTest();
	~ScreenTest();
	void HandleInput(InputState* input);
	bool Initialize();
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
	void SetError(std::string errorMsg);
protected:
	void Draw(SDL_Renderer* renderer);
};

/*********************************************/

#endif
