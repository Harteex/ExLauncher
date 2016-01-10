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
