#include "ScreenSystemDialog.h"
#include "../global.h"
#include "../Views/FramePanel.h"
#include "../Xml.h"

using namespace std;

ScreenSystemDialog::ScreenSystemDialog()
{
	popup = true;
	SetTransitionTime(0.5, 0.2);

	contentView = NULL;
	inputView = NULL;
}

ScreenSystemDialog::~ScreenSystemDialog()
{
	if (contentView != NULL)
		delete contentView;
	contentView = NULL;
}

bool ScreenSystemDialog::Initialize()
{
	contentView = new FramePanel();
	contentView->SetSize(Size(SIZE_FILL_PARENT, SIZE_FILL_PARENT));

	try
	{
		Xml xml;
		contentView->AddChildView(xml.LoadView("data/system/systemMenu.xml"));
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

		selectionLabel = dynamic_cast<Label*>(contentView->GetChildViewById("selectionLabel"));

		if (inputView != NULL)
			inputView->SelectByIndex(1);
	}
	catch (exception& ex)
	{
		lastError = ex.what();
		return false;
	}

	return true;
}

void ScreenSystemDialog::HandleInput(InputState* input)
{
	if (inputView != NULL)
		inputView->HandleInput(input);

	if (input->GameKeyJustPressed(GAMEKEY_B) || input->GameKeyJustPressed(GAMEKEY_START))
		OnEvent(NULL, EventTypeGoBack, "");
}

void ScreenSystemDialog::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);

	if (IsActive())
	{
		//
	}
}

void ScreenSystemDialog::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xA0 * (1 - transitionPosition));
	SDL_RenderFillRect(renderer, NULL);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

	if (contentView != NULL && screenstate != TransitionOff) // Hide immediately when transitioning off
		contentView->Draw();
}

void ScreenSystemDialog::OnEvent(View* sender, EventType eventType, string eventValue, vector<string> eventArgs)
{
	// FIXME catch exceptions for everything in this method
	// TODO clean up this method

	switch (eventType)
	{
	case EventTypeAction:
		{
			/*if (eventValue == "app")
			{
				if (eventArgs.empty())
					return;

				Position senderPos = sender->GetAbsolutePosition();
				Size senderSize = sender->GetCalculatedSize();

				ScreenAppLaunch* appLaunch = new ScreenAppLaunch();
				appLaunch->SetStartRectangle(senderPos.x, senderPos.y, senderSize.w, senderSize.h);
				appLaunch->SetAppId(sender->GetId());
				appLaunch->SetExec(eventArgs);
				screenManager->AddScreen(appLaunch);
			}
			else
			{
				vector<string> action = split(eventValue, ':');
				if (action.size() >= 2)
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
				}
			}*/
			break;
		}

	case EventTypeGoBack:
		{
			ExitScreen();
			break;
		}

	case EventTypeSelectionChanged:
		{
			if (selectionLabel != NULL)
				selectionLabel->SetText(sender->GetName());

			break;
		}
	}
}
