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

#include "ScreenSettingsSelectTheme.h"
#include "../global.h"
#include "../Views/FramePanel.h"
#include "../Xml.h"
#include "ScreenMenu.h"

using namespace std;

ScreenSettingsSelectTheme::ScreenSettingsSelectTheme()
{
	SetTransitionTime(0.0, 0.0);

	contentView = NULL;
	inputView = NULL;
}

ScreenSettingsSelectTheme::~ScreenSettingsSelectTheme()
{
	if (contentView != NULL)
		delete contentView;
	contentView = NULL;
}

bool ScreenSettingsSelectTheme::Initialize(Graphics& graphics)
{
	contentView = new FramePanel();
	contentView->SetSize(Size(SIZE_FILL_PARENT, SIZE_FILL_PARENT));

	try
	{
		Xml xml;
		contentView->AddChildView(xml.LoadView("data/system/settingsSelectTheme.xml"));
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

		//selectionLabel = dynamic_cast<Label*>(contentView->GetChildViewById("selectionLabel"));

		AddThemesToList(v);
		v->InitializeAll(this);
		v->RecalculateLayout();

		if (inputView != NULL)
			inputView->SelectByIndex(0); // FIXME select current theme?
	}
	catch (exception& ex)
	{
		lastError = ex.what();
		return false;
	}

	return true;
}

void ScreenSettingsSelectTheme::AddThemesToList(View* fillView)
{
	map<string, string> fillData;
	auto themes = screenManager->GetThemeManager()->GetAllThemes();
	for (auto theme : themes)
	{
		View* newView = fillView->GetItemTemplate()->Copy();

		fillData["themeName"] = theme.second->GetName();

		newView->SetId(theme.first);
		newView->SetAction("changeTheme");

		newView->FillDataAll(fillData);
		fillView->AddChildView(newView);
	}
}

void ScreenSettingsSelectTheme::HandleInput(InputState* input)
{
	if (inputView != nullptr)
		inputView->HandleInput(input);

	if (input->GameKeyJustPressed(GAMEKEY_B))
		OnEvent(NULL, EventTypeGoBack, "");
}

void ScreenSettingsSelectTheme::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);

	if (IsActive())
	{
		//
	}
}

void ScreenSettingsSelectTheme::Draw(Graphics& graphics)
{
	if (contentView != NULL)
		contentView->Draw();
}

void ScreenSettingsSelectTheme::OnEvent(View* sender, EventType eventType, string eventValue, vector<string> eventArgs)
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
			if (eventValue == "changeTheme")
				SetTheme(sender->GetId());

			break;
		}
	}
}

void ScreenSettingsSelectTheme::SetTheme(string themeId)
{
	Theme* newTheme = screenManager->GetThemeManager()->GetTheme(themeId);
	if (newTheme->GetEntryPoint().empty())
	{
		// FIXME handle error
		return;
	}

	ThemeManager::SetTheme(themeId);
	screenManager->GetThemeManager()->SaveSettings();

	screenManager->ExitAllScreens(); // FIXME don't exit backgroundScreen........
	screenManager->AddScreen(new ScreenMenu(string("@theme/") + newTheme->GetEntryPoint()));
}
