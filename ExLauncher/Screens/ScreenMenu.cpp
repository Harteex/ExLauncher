#include "ScreenMenu.h"
#include <iostream>
#include <fstream>
#include "../global.h"
#include "../Views/GridView.h"
#include "../Views/FramePanel.h"
#include "../Views/Label.h"
#include <string>
#include <exception>
#include <stdlib.h>
#include "../Xml.h"
#include "../utils.h"
#include "../ThemeManager.h"

using namespace std;

ScreenMenu::ScreenMenu(std::string layout)
{
	this->layout = layout;
	contentView = new FramePanel();
	inputView = NULL;
	categoryFillView = NULL;
	itemFillView = NULL;
	canGoBack = false;

	SetTransitionTime(0.6, 0.1);
}

ScreenMenu::~ScreenMenu()
{
}

bool ScreenMenu::Initialize()
{
	contentView->SetSize(Size(SIZE_MATCH_PARENT, SIZE_MATCH_PARENT));

	try
	{
		Xml xml;
		contentView->AddChildView(xml.LoadView(ThemeManager::ProcessPath(layout)));
		contentView->InitializeAll(this);
		contentView->CalculateLayout(Position(0, 0), screenManager->GetDisplaySize());
		contentView->CalculateAbsolutePosition(Position(0, 0));

		View* v = GetViewById("inputView");
		if (v != NULL)
		{
			inputView = dynamic_cast<ISelectionHandler*>(v);
			if (inputView == NULL)
			{
				throw runtime_error("This view cannot be used as an inputView. An inputView must implement ISelectionHandler.");
			}
		}

		categoryFillView = GetFirstViewByTag("fillCategories");
		if (categoryFillView != NULL)
		{
			if (categoryFillView->GetItemTemplate() == NULL)
			{
				throw runtime_error("category view to fill does not have an itemTemplate");
			}

			if (categoryFillView->GetItemTemplate()->GetItemTemplate() == NULL)
			{
				throw runtime_error("itemTemplate of category view to fill does not have an itemTemplate");
			}

			auto tags = categoryFillView->GetTags();
			ParseCategoriesFromTags(tags);
		}
		else
		{
			itemFillView = GetFirstViewByTag("fillItems");

			if (itemFillView != NULL && itemFillView->GetItemTemplate() == NULL)
			{
				throw runtime_error("view to fill does not have an itemTemplate");
			}

			if (itemFillView != NULL)
			{
				auto tags = itemFillView->GetTags();
				ParseCategoriesFromTags(tags);
			}
		}

		HandleApps();
		FillDataArguments();

		if (inputView != NULL)
			inputView->SelectByIndex(0);
	}
	catch (exception& ex)
	{
		lastError = ex.what();
		return false;
	}

	return true;
}

void ScreenMenu::ParseCategoriesFromTags(vector<string> tags)
{
	for (string tag : tags)
	{
		if (tag.substr(0, 10) == "categories")
		{
			size_t sepPos = tag.find_first_of('=');
			if (sepPos != string::npos)
			{
				string categoryListString = tag.substr(sepPos + 1);

				vector<string> categoryList = split(categoryListString, ',');

				for (string cat : categoryList)
				{
					// FIXME trim cat
					if (cat[0] == '-')
					{
						categoriesToExclude.push_back(cat.substr(1));
					}
					else
					{
						categoriesToInclude.push_back(cat);
					}
				}

				break;
			}
		}
	}
}

void ScreenMenu::HandleInput(InputState* input)
{
	if (inputView != NULL)
		inputView->HandleInput(input);

	if (input->GameKeyJustPressed(GAMEKEY_B))
		OnEvent(NULL, EventTypeGoBack, "");

	/*bool virtualKeyboardWasActive = formContainer.IsVirtualKeyboardActive();
	formContainer.HandleInput(input);
	if (formContainer.IsVirtualKeyboardActive() || virtualKeyboardWasActive)
		return;*/
}

void ScreenMenu::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);

	if (IsActive())
	{
		if (contentView != NULL)
			contentView->Update();
	}
}

void ScreenMenu::Draw(SDL_Renderer* renderer)
{
	// Antingen detta... eller:
	// vanlig fade time både in och ut, och alla fadeas samtidigt utan delay

	/*if (screenstate == TransitionOn)
		formContainer.SetAlpha(GetTransitionAlpha(), 10, 80);
	else
		formContainer.SetAlpha(GetTransitionAlpha());

	formContainer.Draw(renderer, !otherScreenHasFocus && screenstate != TransitionOff);*/

	if (contentView != NULL)
		contentView->Draw();

	/*if (titleTextureBg != NULL)
		drawTexture(8, 3, titleTextureBg, renderer);

	if (titleTexture != NULL)
		drawTexture(8, 3, titleTexture, renderer);*/
}

View* ScreenMenu::GetViewById(std::string id)
{
	if (contentView == NULL)
		return NULL;
	else
		return contentView->GetChildViewById(id);
}

View* ScreenMenu::GetFirstViewByTag(std::string tag)
{
	if (contentView == NULL)
		return NULL;
	else
		return contentView->GetFirstChildViewByTag(tag);
}

bool ScreenMenu::ShouldCategoryBeIncluded(string category)
{
	if (categoriesToInclude.empty())
	{
		bool foundInExclude = (std::find(categoriesToExclude.begin(), categoriesToExclude.end(), category) != categoriesToExclude.end());
		if (foundInExclude)
			return false;
	}
	else
	{
		bool foundInInclude = (std::find(categoriesToInclude.begin(), categoriesToInclude.end(), category) != categoriesToInclude.end());
		if (!foundInInclude)
			return false;
	}

	return true;
}

void ScreenMenu::HandleApps()
{
	map<string, vector<App*>*>* apps = screenManager->GetAppManager()->GetAllApps();
	for (auto kv : *apps)
	{
		for (App* app : *kv.second)
		{
			AddApp(app, kv.first);
		}
	}

	EndUpdateApps();
}

View* ScreenMenu::FindOrCreateCategoryView(string category)
{
	View* categoryView = NULL;
	for (int i = 0; i < categoryFillView->GetNumberOfChildren(); i++)
	{
		View* c = categoryFillView->GetChildView(i);
		if (c->GetName() == category)
		{
			categoryView = c;
			break;
		}
	}

	if (categoryView == NULL)
	{
		categoryView = categoryFillView->GetItemTemplate()->Copy();
		categoryView->InitializeAll(this);
		categoryView->SetName(category);

		categoryFillView->AddChildView(categoryView);
	}

	return categoryView;
}

void ScreenMenu::AddApp(App* app, string category)
{
	if (!ShouldCategoryBeIncluded(category))
		return;

	if (categoryFillView != NULL)
	{
		View* categoryView = FindOrCreateCategoryView(category);
		AddViewForApp(categoryView, app);
	}
	else if (itemFillView != NULL)
	{
		AddViewForApp(itemFillView, app);
	}
}

void ScreenMenu::StartUpdateApps()
{
	// Save current selection(s)?
}

// FIXME get previous selection, and try to select this again if it's not removed. Maybe use the id field for this purpose.
// Or maybe do that in startupdateapps? dunno
// Also, try to optimize... do we really need to filldatainview? (should only be done if a new category is created I think?)
void ScreenMenu::EndUpdateApps()
{
	if (categoryFillView != NULL)
	{
		for (int i = 0; i < categoryFillView->GetNumberOfChildren(); i++)
		{
			View* categoryView = categoryFillView->GetChildView(i);

			SortItemsByName(categoryView);

			ISelectionHandler* selectionHandler = dynamic_cast<ISelectionHandler*>(categoryView);
			if (selectionHandler != NULL)
				selectionHandler->SelectByIndex(0);

			FillDataInView(categoryView, arguments->GetStringMap());
		}

		categoryFillView->RecalculateLayout();
	}
	else if (itemFillView != NULL)
	{
		SortItemsByName(itemFillView);

		ISelectionHandler* selectionHandler = dynamic_cast<ISelectionHandler*>(itemFillView);
		if (selectionHandler != NULL)
			selectionHandler->SelectByIndex(0);

		FillDataInView(itemFillView, arguments->GetStringMap());
		itemFillView->RecalculateLayout();
	}
}

void ScreenMenu::RemoveApp(string id)
{
	// TODO implement
}

void ScreenMenu::AddViewForApp(View* fillView, App* app)
{
	View* newView = fillView->GetItemTemplate()->Copy();

	newView->InitializeAll(this);
	FillDataInView(newView, app->GetAllData());
	newView->SetName(app->GetData("name", ""));

	fillView->AddChildView(newView);
}

void ScreenMenu::FillDataArguments()
{
	FillDataInView(contentView, arguments->GetStringMap());
}

void ScreenMenu::FillDataInView(View* v, map<string, string> data)
{
	v->FillData(data);

	for (int i = 0; i < v->GetNumberOfChildren(); i++)
	{
		View* c = v->GetChildView(i);
		FillDataInView(c, data);
	}
}

bool _SortItemsByNameItemComparer(View* v1, View* v2)
{
	return v1->GetName().compare(v2->GetName()) < 0;
}

void ScreenMenu::SortItemsByName(View* containingView)
{
	containingView->SortChildren(_SortItemsByNameItemComparer);
}

void ScreenMenu::OnEvent(View* sender, EventType eventType, string eventValue)
{
	// FIXME catch exceptions for everything in this method

	switch (eventType)
	{
	case EventTypeAction:
		{
			vector<string> action = split(eventValue, ':');
			if (action.size() == 2 || action.size() == 3)
			{
				if (action[0] == "screen")
				{
					ScreenMenu* newScreen = new ScreenMenu(action[1]);

					// If we have arguments, parse them and set them on the launching screen
					if (action.size() == 3)
					{
						vector<string> args = split(action[2], ';');
						for (string arg : args)
						{
							size_t separationIndex = arg.find_first_of('=');
							if (separationIndex == string::npos)
								throw runtime_error("invalid arguments");

							newScreen->arguments->PutString(arg.substr(0, separationIndex), arg.substr(separationIndex + 1));
						}
					}

					newScreen->SetCanGoBack(true);

					screenManager->AddScreen(newScreen);
				}
				else if (action[0] == "app")
				{
					// TODO Launch app

					// TODO Animate launching from senders position
				}
			}
		}

		break;
	case EventTypeGoBack:
		if (canGoBack)
			ExitScreen();
		break;
	}
}

bool ScreenMenu::GetCanGoBack()
{
	return canGoBack;
}

void ScreenMenu::SetCanGoBack(bool canGoBack)
{
	this->canGoBack = canGoBack;
}
