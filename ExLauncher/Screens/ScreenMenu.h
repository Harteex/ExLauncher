#ifndef _SCREEN_MENU_H_
#define _SCREEN_MENU_H_

/*********************************************/

#include "../ScreenSystem/Screen.h"
#include "../ScreenSystem/ScreenManager.h"
#include "../ViewSystem/View.h"
#include "../ViewSystem/ISelectionHandler.h"
#include "../Views/Image.h"
#include <list>

/*********************************************/

class ScreenMenu : public Screen//, IFormEvent
{
private:
	std::string layout;
	View* contentView;
	ISelectionHandler* inputView;
	View* categoryFillView;
	View* itemFillView;
	std::string title;
	SDL_Texture* titleTexture;
	SDL_Texture* titleTextureBg;
	std::map<std::string, std::string> fillData;
	bool canGoBack;

	//std::list<View*> itemFillViews;
public:
	ScreenMenu(std::string layout);
	virtual ~ScreenMenu();
	void HandleInput(InputState* input);
	bool Initialize();
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
	void SetTitle(std::string title);
	View* GetViewById(std::string id);
	View* GetFirstViewByTag(std::string tag);
	void HandleApps();
	void AddApp(View* fillView, App* app);
	void FillItem(View* v, std::map<std::string, std::string> data);
	void OnEvent(View* sender, EventType eventType, std::string eventValue);
	bool GetCanGoBack();
	void SetCanGoBack(bool canGoBack);
protected:
	//FormContainer formContainer;

	void Draw(SDL_Renderer* renderer);
	//virtual void OnFormEvent(int position, FormEventType eventType, std::string tag) = 0;
};

/*********************************************/

#endif
