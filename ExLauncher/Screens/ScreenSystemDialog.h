#ifndef _SCREEN_SYSTEM_DIALOG_H_
#define _SCREEN_SYSTEM_DIALOG_H_

/*********************************************/

#include "../ScreenSystem/Screen.h"
#include "../ScreenSystem/ScreenManager.h"
#include "../ViewSystem/ISelectionHandler.h"
#include "../Views/Label.h"

/*********************************************/

class ScreenSystemDialog : public Screen
{
private:
	View* contentView;
	ISelectionHandler* inputView;
	Label* selectionLabel;
public:
	ScreenSystemDialog();
	~ScreenSystemDialog();
	void HandleInput(InputState* input);
	bool Initialize();
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
	void OnEvent(View* sender, EventType eventType, std::string eventValue, std::vector<std::string> eventArgs = std::vector<std::string>());
protected:
	void Draw(SDL_Renderer* renderer);
};

/*********************************************/

#endif
