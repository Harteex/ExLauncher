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

bool ScreenSystemInfo::Initialize(Graphics& graphics)
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

void ScreenSystemInfo::Draw(Graphics& graphics)
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
