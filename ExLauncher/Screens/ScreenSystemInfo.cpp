#include "ScreenSystemInfo.h"
#include "../global.h"
#include "../Views/FramePanel.h"
#include "../Xml.h"

using namespace std;

ScreenSystemInfo::ScreenSystemInfo()
{
	SetTransitionTime(0.0, 0.0);

	contentView = NULL;
}

ScreenSystemInfo::~ScreenSystemInfo()
{
	if (contentView != NULL)
		delete contentView;
	contentView = NULL;
}

bool ScreenSystemInfo::Initialize()
{
	contentView = new FramePanel();
	contentView->SetSize(Size(SIZE_FILL_PARENT, SIZE_FILL_PARENT));

	Label* label = new Label();
	label->SetSize(Size(SIZE_FILL_PARENT, SIZE_FILL_PARENT));
	label->SetGravity(GRAVITY_HCENTER | GRAVITY_VCENTER);
	label->SetText("System info N/A");

	contentView->AddChildView(label);

	contentView->InitializeAll(this);
	contentView->CalculateLayout(screenManager->GetDisplaySize());
	contentView->CalculateAbsolutePosition(Position(0, 0));

	return true;
}

void ScreenSystemInfo::HandleInput(InputState* input)
{
	if (input->GameKeyJustPressed(GAMEKEY_B))
		OnEvent(NULL, EventTypeGoBack, "");
}

void ScreenSystemInfo::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);

	if (IsActive())
	{
		//
	}
}

void ScreenSystemInfo::Draw(SDL_Renderer* renderer)
{
	if (contentView != NULL)
		contentView->Draw();
}

void ScreenSystemInfo::OnEvent(View* sender, EventType eventType, string eventValue, vector<string> eventArgs)
{
	switch (eventType)
	{
	case EventTypeGoBack:
		{
			ExitScreen();
			break;
		}
	}
}
