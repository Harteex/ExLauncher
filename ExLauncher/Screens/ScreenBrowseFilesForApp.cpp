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

#include "ScreenBrowseFilesForApp.h"
#include "../global.h"
#include "../Views/FramePanel.h"
#include "../Xml.h"
#include "ScreenMenu.h"
#include "ScreenAppLaunch.h"
#include "../utils.h"
#include "../Filesystem/FilesystemUtils.h"

using namespace std;

ScreenBrowseFilesForApp::ScreenBrowseFilesForApp()
{
	SetTransitionTime(0.0, 0.0);

	contentView = NULL;
	browseView = NULL;
	inputView = NULL;
	currentDirectory = "";
}

ScreenBrowseFilesForApp::~ScreenBrowseFilesForApp()
{
	if (contentView != NULL)
		delete contentView;
	contentView = NULL;
}

void ScreenBrowseFilesForApp::SetAppId(std::string appId)
{
	this->appId = appId;
}

bool ScreenBrowseFilesForApp::Initialize(Graphics& graphics)
{
	contentView = new FramePanel();
	contentView->SetSize(Size(SIZE_FILL_PARENT, SIZE_FILL_PARENT));

	try
	{
		Xml xml;
		contentView->AddChildView(xml.LoadView("data/system/browseFilesForAppDefault.xml"));
		contentView->InitializeAll(this);
		contentView->CalculateLayout(screenManager->GetDisplaySize());
		contentView->CalculateAbsolutePosition(Position(0, 0));

		View* v = contentView->GetChildViewById("inputView");
		if (v != NULL)
		{
			inputView = dynamic_cast<ISelectionHandler*>(v);
			if (inputView == NULL)
			{
				throw runtime_error("This view cannot be used as an inputView. An inputView must implement ISelectionHandler.");
			}
		}

		browseView = v;

		App* app = GetAppManager()->GetApp(appId);
		if (app != nullptr)
		{
			contentView->FillDataAll(app->GetAllData());
		}

		mimeTypes.clear();
		string mimeTypesStr = app->GetData("MimeType", "");
		string mimeTypesStrTrimmed = trim(mimeTypesStr, ";");
		mimeTypes = split(mimeTypesStrTrimmed, ';');

		currentDirectory = GetStartDirectory();
		ListDirectory(currentDirectory, v);
	}
	catch (exception& ex)
	{
		lastError = ex.what();
		return false;
	}

	return true;
}

std::string ScreenBrowseFilesForApp::GetStartDirectory()
{
#ifdef WINDOWS
	string startDirectory = "C:/";
#endif

#ifdef UNIX
	string startDirectory = "/usr/local/home/";
#endif

	return startDirectory;
}

void ScreenBrowseFilesForApp::ListDirectory(string directory, View* fillView, string folderToSelect)
{
	while (fillView->GetNumberOfChildren() > 0)
		fillView->DeleteChildView(0);

	vector<string> directories = FilesystemUtils::GetDirectories(directory);
	vector<string> files = FilesystemUtils::GetFilesByMimeTypes(directory, mimeTypes);

	if (!IsRootFolder(directory))
		directories.insert(directories.begin(), "..");

	map<string, string> fillData;
	int selection = 0;
	
	int i = 0;
	for (string directory : directories)
	{
		View* newView = fillView->GetItemTemplate()->Copy();

		fillData["fileName"] = directory;

		newView->SetName(directory);
		newView->SetAction("cd");
		newView->SetActionArgs({directory});

		newView->FillDataAll(fillData);
		fillView->AddChildView(newView);

		if (directory == folderToSelect)
			selection = i;

		i++;
	}
	for (string file : files)
	{
		View* newView = fillView->GetItemTemplate()->Copy();

		fillData["fileName"] = file;

		newView->SetName(file);
		newView->SetAction("file");
		newView->SetActionArgs({ file });

		newView->FillDataAll(fillData);

		View* icon = newView->GetFirstChildViewByTag("fileTypeIcon");
		if (icon != NULL)
			icon->GetParentView()->DeleteChildView(icon);

		fillView->AddChildView(newView);
	}

	fillView->InitializeAll(this);
	fillView->RecalculateLayout();

	if (inputView != NULL)
		inputView->SelectByIndex(selection);
}

bool ScreenBrowseFilesForApp::IsRootFolder(std::string directory)
{
#ifdef WINDOWS
	if (directory.size() <= 3)
		return true;
#endif

#ifdef UNIX
	if (directory == "/")
		return true;
#endif

	return false;
}

void ScreenBrowseFilesForApp::HandleInput(InputState* input)
{
	if (inputView != nullptr)
		inputView->HandleInput(input);

	if (input->GameKeyJustPressed(GAMEKEY_B))
		OnEvent(NULL, EventTypeGoBack, "");

	if (input->GameKeyJustPressed(GAMEKEY_Y))
		OnEvent(NULL, EventTypeAction, "cd", { ".." });
}

void ScreenBrowseFilesForApp::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);

	if (IsActive())
	{
		if (contentView != nullptr)
			contentView->Update();
	}
}

void ScreenBrowseFilesForApp::Draw(Graphics& graphics)
{
	if (contentView != NULL)
		contentView->Draw();
}

void ScreenBrowseFilesForApp::OnEvent(View* sender, EventType eventType, string eventValue, vector<string> eventArgs)
{
	switch (eventType)
	{
	case EventTypeGoBack:
		{
			ExitScreen();
			break;
		}
	case EventTypeAction:
		{
			if (eventValue == "cd")
			{
				if (eventArgs[0] == "..")
				{
					if (!IsRootFolder(currentDirectory))
					{
						currentDirectory.pop_back();
						auto found = currentDirectory.find_last_of("/\\");
						string folderName = currentDirectory.substr(found + 1);
						currentDirectory = currentDirectory.substr(0, found + 1);

						ListDirectory(currentDirectory, browseView, folderName);
					}
				}
				else
				{
					currentDirectory += eventArgs[0] + "/";
					ListDirectory(currentDirectory, browseView);
				}
			}
			else if (eventValue == "file")
			{
				// Since it's a file, we want to launch the chosen app with this file
				if (appId.size() > 0)
				{
					App* app = GetAppManager()->GetApp(appId);
					if (app != nullptr)
					{
						vector<string> exec = app->GetExec();
						if (exec.size() > 0)
						{
							string filePath = currentDirectory + eventArgs[0];
							exec.push_back(filePath);

							// Launch app
							Position senderPos = sender->GetAbsolutePosition();
							Size senderSize = sender->GetCalculatedSize();

							ScreenAppLaunch* appLaunch = new ScreenAppLaunch();
							appLaunch->SetStartRectangle(senderPos.x, senderPos.y, senderSize.w, senderSize.h);
							appLaunch->SetAppId(appId, filePath);
							appLaunch->SetExec(exec);
							screenManager->AddScreen(appLaunch);
						}
					}
				}
				
				// TODO save directory for this app id
				// TODO modify saving to recent list, so that we save this particular file
			}

			break;
		}
	}
}
