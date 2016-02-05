#ifndef _SCREEN_SYSTEM_INFO_H_
#define _SCREEN_SYSTEM_INFO_H_

/*********************************************/

#include "../ScreenSystem/Screen.h"
#include "../ScreenSystem/ScreenManager.h"
#include "../ViewSystem/ISelectionHandler.h"
#include "../Views/Label.h"

/*********************************************/

class ScreenSystemInfo : public Screen
{
private:
	View* contentView;
public:
	ScreenSystemInfo();
	~ScreenSystemInfo();
	void HandleInput(InputState* input);
	bool Initialize();
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
	void OnEvent(View* sender, EventType eventType, std::string eventValue, std::vector<std::string> eventArgs = std::vector<std::string>());
protected:
	void Draw(SDL_Renderer* renderer);
};

/*********************************************/

#endif
