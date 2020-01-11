/*
Copyright 2018 Andreas Bjerkeholt

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

#ifndef _SCREEN_BROWSE_FILES_FOR_APP_H_
#define _SCREEN_BROWSE_FILES_FOR_APP_H_

/*********************************************/

#include "../ScreenSystem/Screen.h"
#include "../ScreenSystem/ScreenManager.h"
#include "../ViewSystem/ISelectionHandler.h"
#include "../Views/Label.h"

/*********************************************/

class ScreenBrowseFilesForApp : public Screen
{
private:
	View* contentView;
	View* browseView;
	ISelectionHandler* inputView;
	std::string currentDirectory;
	std::string appId;
	std::vector<std::string> mimeTypes;

	std::string GetStartDirectory();
	void ListDirectory(std::string directory, View* fillView, std::string folderToSelect = "");
	bool IsRootFolder(std::string directory);
public:
	ScreenBrowseFilesForApp();
	~ScreenBrowseFilesForApp();

	void SetAppId(std::string appId);
	void HandleInput(InputState* input);
	bool Initialize(Graphics& graphics);
	void Update(bool otherScreenHasFocus, bool coveredByOtherScreen);
	void OnEvent(View* sender, EventType eventType, std::string eventValue, std::vector<std::string> eventArgs = std::vector<std::string>());
protected:
	void Draw(Graphics& graphics);
};

/*********************************************/

#endif
