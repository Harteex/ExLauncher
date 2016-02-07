/*
Copyright 2016 Andreas Bjerkeholt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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

class ScreenMenu : public Screen
{
private:
	std::string layout;
	View* contentView;
	ISelectionHandler* inputView;
	View* categoryFillView;
	View* itemFillView;
	std::list<std::string> categoriesToInclude;
	std::list<std::string> categoriesToExclude;
	std::map<std::string, std::string> fillData;
	bool canGoBack;

	View* FindOrCreateCategoryView(std::string category);
	bool ShouldCategoryBeIncluded(std::string category);
public:
	ScreenMenu(std::string layout);
	virtual ~ScreenMenu();
	void HandleInput(InputState* input);
	bool Initialize();
	void ParseCategoriesFromTags(std::vector<std::string> tags);
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
	View* GetViewById(std::string id);
	View* GetFirstViewByTag(std::string tag);
	bool HandleApps();
	bool AddApp(App* app, std::string category);
	void StartUpdateApps();
	void EndUpdateApps();
	void RemoveApp(std::string id);
	bool AddViewForApp(View* fillView, App* app);
	void FillDataArguments();
	void FillDataInView(View* v, std::map<std::string, std::string> data);
	void SortItemsByName(View* containingView);
	void OnEvent(View* sender, EventType eventType, std::string eventValue, std::vector<std::string> eventArgs = std::vector<std::string>());
	bool GetCanGoBack();
	void SetCanGoBack(bool canGoBack);
protected:
	void Draw(SDL_Renderer* renderer);
};

/*********************************************/

#endif
