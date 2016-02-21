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

#include "ListView.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "FramePanel.h"

using namespace std;

ListView::ListView()
{
	contentSize = Size(0, 0);
	orientation = OrientationHorizontal;
	itemSize = 0;
}

ListView::~ListView()
{
}

void ListView::OnInitialize()
{
	ScrollView::SetOrientation(orientation);
}

void ListView::OnLayoutChange()
{
	Position childOffset = Position(0, 0);
	contentSize = Size(0, 0);

	if ((size.w == SIZE_FILL_PARENT || size.w > 0) && orientation == OrientationVertical)
		contentSize.w = calculatedSize.w;
	if ((size.h == SIZE_FILL_PARENT || size.h > 0) && orientation == OrientationHorizontal)
		contentSize.h = calculatedSize.h;

	if (orientation == OrientationHorizontal)
	{
		if (itemSize <= 0)
			throw runtime_error("item width missing or invalid");

		if (size.h == SIZE_WRAP_CONTENT)
			throw runtime_error("list height cannot depend on content when orientation is horizontal");

		if (size.w == SIZE_WRAP_CONTENT)
			contentSize.w = itemSize * GetNumberOfChildren();
	}
	else if (orientation == OrientationVertical)
	{
		if (itemSize <= 0)
			throw runtime_error("item height missing or invalid");

		if (size.w == SIZE_WRAP_CONTENT)
			throw runtime_error("list width cannot depend on content when orientation is vertical");

		if (size.h == SIZE_WRAP_CONTENT)
			contentSize.h = itemSize * GetNumberOfChildren();
	}

	Size sizeAvailableForChild = Size(itemSize, itemSize);
	if (orientation == OrientationHorizontal)
		sizeAvailableForChild.h = calculatedSize.h;
	else
		sizeAvailableForChild.w = calculatedSize.w;

	for (int i = 0; i < children.size(); i++)
	{
		View* v = children.at(i);

		// TODO Possibly wait with doing this until necessary (lazy loading). Do it on draw instead, if it's not initialized.
		v->CalculateLayout(sizeAvailableForChild);

		if (orientation == OrientationHorizontal)
			v->SetRelativePosition(i * sizeAvailableForChild.w, 0);
		else
			v->SetRelativePosition(0, i * sizeAvailableForChild.h);
	}
}

View* ListView::Copy()
{
	ListView* view = new ListView();

	CopyBase(view);
	view->SetOrientation(orientation);
	view->SetItemSize(itemSize);

	return view;
}

void ListView::AddChildView(View* view)
{
	// Instead of duplicating much layouting logic for children in ListView, wrap children in a FramePanel
	FramePanel* framePanel = new FramePanel();
	framePanel->SetSize(SIZE_FILL_PARENT, SIZE_FILL_PARENT);
	framePanel->SetAction(view->GetAction());
	framePanel->SetActionArgs(view->GetActionArgs());
	framePanel->SetName(view->GetName());
	framePanel->SetId(view->GetId());

	view->SetId("");

	framePanel->AddChildView(view);
	View::AddChildView(framePanel);
}

Orientation ListView::GetOrientation()
{
	return orientation;
}

void ListView::SetOrientation(Orientation orientation)
{
	this->orientation = orientation;
}

int ListView::GetItemSize()
{
	return itemSize;
}

void ListView::SetItemSize(int itemSize)
{
	this->itemSize = itemSize;
}

bool ListView::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	if (name == "orientation")
	{
		if (value == "horizontal")
			SetOrientation(OrientationHorizontal);
		else if (value == "vertical")
			SetOrientation(OrientationVertical);
		else
			throw runtime_error("invalid orientation");

		return true;
	}
	else if (name == "itemSize")
	{
		stringstream ss(value);
		if ((ss >> itemSize).fail() || !(ss >> std::ws).eof())
		{
			throw runtime_error("could not parse itemSize");
		}

		return true;
	}

	return false;
}

Position ListView::GetPositionForItemIndex(int elementNo)
{
	Position position = Position(-1, -1);

	if (elementNo < 0 || elementNo >= children.size())
		return position;

	if (orientation == OrientationHorizontal)
	{
		position.x = elementNo;
		position.y = 0;
	}
	else
	{
		position.x = 0;
		position.y = elementNo;
	}

	return position;
}

int ListView::GetItemIndexForPosition(Position position)
{
	if (orientation == OrientationHorizontal)
	{
		if (position.x < 0 || position.x >= children.size())
			return -1;

		if (position.y != 0)
			return -1;

		return position.x;
	}
	else
	{
		if (position.x != 0)
			return -1;

		if (position.y < 0 || position.y >= children.size())
			return -1;

		return position.y;
	}
}

View* ListView::GetSelectedItem()
{
	return children[selectedIndex];
}

View* ListView::SelectNext(Direction direction)
{
	if (children.size() == 0)
		return NULL;

	int dx = 0;
	int dy = 0;

	switch (direction)
	{
	case DirectionUp:
		dy = -1;
		break;
	case DirectionDown:
		dy = 1;
		break;
	case DirectionLeft:
		dx = -1;
		break;
	case DirectionRight:
		dx = 1;
		break;
	}

	int index = selectedIndex;

	if (orientation == OrientationHorizontal)
	{
		if (selectedIndex + dx < 0 || selectedIndex + dx >= children.size())
			return NULL;

		if (dy != 0)
			return NULL;

		index += dx;
	}

	if (orientation == OrientationVertical)
	{
		if (dx != 0)
			return NULL;

		if (selectedIndex + dy < 0 || selectedIndex + dy >= children.size())
			return NULL;

		index += dy;
	}

	// A new selection was made
	SelectByIndex(index);

	return children[index];
}

View* ListView::Select(Position position)
{
	PropagateStateChange("stateSelected", "false");

	int index = GetItemIndexForPosition(position);
	if (index >= 0 && index < children.size())
	{
		children[index]->PropagateStateChange("stateSelected", "true");
		ScrollTo(children[index]);
		return children[index];
	}
	else
	{
		return NULL;
	}
}

void ListView::OnSelectionChanged()
{
	ScrollTo(children[selectedIndex]);
}
