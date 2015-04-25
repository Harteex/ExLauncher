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

using namespace std;

ScreenMenu::ScreenMenu()
{
	contentView = new FramePanel();
	inputView = NULL;
	itemFillView = NULL;
	title = "";
	titleTexture = NULL;
	titleTextureBg = NULL;

	SetTransitionTime(0.6, 0.1);
}

ScreenMenu::~ScreenMenu()
{
	if (titleTexture != NULL)
		SDL_DestroyTexture(titleTexture);

	if (titleTextureBg != NULL)
		SDL_DestroyTexture(titleTextureBg);
}

bool ScreenMenu::Initialize()
{
	/*formContainer.Initialize(view, screenManager,
		screenManager->GetResourceManager()->GetTTFFont("guiFont"),
		screenManager->GetResourceManager()->GetTTFFont("guiFontSmall"),
		screenManager->GetResourceManager()->GetFont("dialogFontSmall"));
	formContainer.SetEventListener(this);*/

	/*tileAdapter = new ItemAdapter(&formContainer, screenManager->GetRenderer(), tileFont);
	tileAdapter->AddItem<Tile>(new Item("No Recent"));
	tileAdapter->AddItem<Tile>(new Item("No Recent"));
	tileAdapter->AddItem<Tile>(new Item("No Recent"));
	tileAdapter->AddItem<Tile>(new Item("No Recent"));
	tileAdapter->AddItem<Tile>(new Item("Games", screenManager->GetResourceManager()->GetImage("IconGames")));
	tileAdapter->AddItem<Tile>(new Item("Emulators", screenManager->GetResourceManager()->GetImage("IconEmulators")));
	tileAdapter->AddItem<Tile>(new Item("Applications", screenManager->GetResourceManager()->GetImage("IconApplications")));
	tileAdapter->AddItem<Tile>(new Item("Settings", screenManager->GetResourceManager()->GetImage("IconSettings")));
	//tileAdapter->AddItem<TextField>(new Item());
	*/

	contentView->SetSize(Size(SIZE_MATCH_PARENT, SIZE_MATCH_PARENT));

	try
	{
		Xml xml;
		contentView->AddChildView(xml.LoadView("data/themes/default/test.xml", true));
		contentView->InitializeAll(screenManager->GetResourceManager(), screenManager->GetRenderer());
		contentView->CalculateLayout(Position(0, 0), screenManager->GetDisplaySize());
		contentView->CalculateAbsolutePosition(Position(0, 0));

		inputView = GetViewById("inputView");
		itemFillView = GetFirstViewByTag("fillItems");

		if (itemFillView != NULL)
		{
			View* itemTemplate = itemFillView->GetItemTemplate();
			if (itemTemplate == NULL)
			{
				throw runtime_error("view to fill does not have an itemTemplate");
			}
			else
			{
				vector<App*>* apps = screenManager->GetAppManager()->GetApps("games");
				for (int i = 0; i < apps->size(); i++)
				{
					AddApp(apps->at(i));
				}

				if (!apps->empty())
					itemFillView->RecalculateLayout();
			}
		}

		if (inputView != NULL)
			inputView->PropagateStateChange("stateSelected", "false");

		// TEST
		/*if (inputView != NULL)
			inputView->GetChildView(3)->PropagateStateChange("stateSelected", "true");*/
	}
	catch (exception& ex)
	{
		lastError = ex.what();
		return false;
	}

	SetTitle("START");

	return true;
}

void ScreenMenu::HandleInput(InputState* input)
{
	if (inputView != NULL)
		inputView->HandleInput(input);

	/*bool virtualKeyboardWasActive = formContainer.IsVirtualKeyboardActive();
	formContainer.HandleInput(input);
	if (formContainer.IsVirtualKeyboardActive() || virtualKeyboardWasActive)
		return;

	if (input->GameKeyJustPressed(GAMEKEY_UP))
		formContainer.SelectNextElement(DirectionUp);
	else if (input->GameKeyJustPressed(GAMEKEY_DOWN))
		formContainer.SelectNextElement(DirectionDown);
	else if (input->GameKeyJustPressed(GAMEKEY_LEFT))
		formContainer.SelectNextElement(DirectionLeft);
	else if (input->GameKeyJustPressed(GAMEKEY_RIGHT))
		formContainer.SelectNextElement(DirectionRight);*/
	/*else if (input->GameKeyJustPressed(GAMEKEY_B))
		formContainer.FireEvent("", FormEventBack);*/

	// Test code
	/*if (input->GameKeyJustPressed(GAMEKEY_UP))
	{
		View* v = GetViewById("testlabel");
		if (v != NULL)
			((Label*)v)->SetText("omglol");
	}*/
}

void ScreenMenu::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);

	if (IsActive())
	{
		if (contentView != NULL)
			contentView->Update();
		//formContainer.Update();
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
		contentView->Draw(renderer);

	/*if (titleTextureBg != NULL)
		drawTexture(8, 3, titleTextureBg, renderer);

	if (titleTexture != NULL)
		drawTexture(8, 3, titleTexture, renderer);*/
}

void ScreenMenu::SetTitle(string title)
{
	this->title = title;

	TTF_Font* font = screenManager->GetResourceManager()->LoadTTFFont("titleFont", "data/fonts/OpenSans-Light.ttf", 22); // OpenSans-Light // OpenSans-Regular
	titleTexture = drawText(title.c_str(), 0xff, 0xff, 0xff, font, screenManager->GetRenderer());

	SDL_Surface* blackText = drawTextSurface(title.c_str(), 0x00, 0x00, 0x00, font);
	SDL_Surface* blurredText = blurSurface(blackText, 2);
	titleTextureBg = SDL_CreateTextureFromSurface(screenManager->GetRenderer(), blurredText);
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

void ScreenMenu::AddApp(App* app)
{
	View* newView = itemFillView->GetItemTemplate()->Copy();
	FillData(newView, app->GetAllData());

	newView->InitializeAll(screenManager->GetResourceManager(), screenManager->GetRenderer());
	itemFillView->AddChildView(newView);
}

void ScreenMenu::FillData(View* v, map<string, string> data)
{
	v->FillData(data);

	for (int i = 0; i < v->GetNumberOfChildren(); i++)
	{
		View* c = v->GetChildView(i);
		FillData(c, data);
	}
}

/*void ScreenMainMenu::OnFormEvent(int position, FormEventType eventType, std::string tag)
{
	if (eventType == FormEventPressed)
	{
		if (position < 4)
		{
			//
		}
		else
		{
			screenManager->AddScreen(new ScreenAppsCollection(tag));
		}
	}
}*/