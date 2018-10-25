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

#include "CollectionView.h"
#include <algorithm>
#include "../ScreenSystem/Screen.h"

using namespace std;

CollectionView::CollectionView()
{
	contentSize.w = 0;
	contentSize.h = 0;

	selectedIndex = 0;
}

void CollectionView::HandleInput(InputState* input)
{
	if (input->GameKeyRepeat(GAMEKEY_UP))
		SelectNext(DirectionUp);
	else if (input->GameKeyRepeat(GAMEKEY_DOWN))
		SelectNext(DirectionDown);
	else if (input->GameKeyRepeat(GAMEKEY_LEFT))
		SelectNext(DirectionLeft);
	else if (input->GameKeyRepeat(GAMEKEY_RIGHT))
		SelectNext(DirectionRight);
	else if (input->GameKeyJustPressed(GAMEKEY_A))
	{
		View* sender = GetSelectedView();
		if (sender != NULL && sender->GetAction() != "")
			context->OnEvent(sender, EventTypeAction, GetSelectedView()->GetAction(), GetSelectedView()->GetActionArgs());
	}
}

int CollectionView::GetSelectedIndex()
{
	return selectedIndex;
}

View* CollectionView::GetSelectedView()
{
	if (selectedIndex < 0 || selectedIndex >= children.size())
		return NULL;

	return children[selectedIndex];
}

bool CollectionView::SelectByName(string name)
{
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children[i];

		if (v->GetName() == name)
			return SelectByIndex(i);
	}

	// No view with the specified name was found
	return false;
}

bool CollectionView::SelectByIndex(int index)
{
	if (children.size() == 0)
	{
		selectedIndex = 0;
		return false; // No real selection has been made except for resetting the variable
	}

	if (index < 0 || index >= children.size())
		return false;

	PropagateStateChange("stateSelected", "false");
	children[index]->PropagateStateChange("stateSelected", "true");

	selectedIndex = index;
	OnSelectionChanged();

	View* selectedView = GetSelectedView();
	if (selectedView != NULL)
		context->OnEvent(selectedView, EventTypeSelectionChanged, "");

	return true;
}

bool CollectionView::SelectById(std::string id)
{
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children[i];

		if (v->GetId() == id)
			return SelectByIndex(i);
	}

	// No view with the specified name was found
	return false;
}
