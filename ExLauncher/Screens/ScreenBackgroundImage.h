#ifndef _SCREEN_BACKGROUND_IMAGE_H_
#define _SCREEN_BACKGROUND_IMAGE_H_

/*********************************************/

#include "../ScreenSystem/Screen.h"
#include "../ScreenSystem/ScreenManager.h"
#include <vector>
#include <SDL.h>

/*********************************************/

class ScreenBackgroundImage : public Screen
{
private:
	std::vector<SDL_Texture*> images;
	int curImage;
	int curTime;
	int timeBetweenImages;
	bool isInTransition;
	int transitionTime;
public:
	ScreenBackgroundImage();
	void SetTimeBetweenImages(int seconds);
	void SetTransitionTime(int seconds);
	void HandleInput(InputState* input);
	bool Initialize();
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
	void AddImage(std::string id);
protected:
	void Draw(SDL_Renderer* renderer);
};

/*********************************************/

#endif
