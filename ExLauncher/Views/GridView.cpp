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

#include "GridView.h"
#include <sstream>

using namespace std;

GridView::GridView()
{
	contentSize = Size(0, 0);
	itemCountSecondaryDirection = 1;

	orientation = OrientationHorizontal;
	gridSpacing = Size(0, 0);
	itemSize = Size(0, 0);
	leadingContentInset = 0;
	trailingContentInset = 0;
}

GridView::~GridView()
{
}

void GridView::OnInitialize()
{
	ScrollView::SetOrientation(orientation);
	ScrollView::SetContentInset(leadingContentInset, trailingContentInset);
}

void GridView::OnLayoutChange()
{
	if (itemSize.w <= 0)
		throw runtime_error("GridView: itemWidth must be greater than zero");

	if (itemSize.h <= 0)
		throw runtime_error("GridView: itemHeight must be greater than zero");

	if (orientation == OrientationHorizontal)
	{
		if (size.h == SIZE_WRAP_CONTENT)
			throw runtime_error("grid height cannot depend on content");

		itemCountSecondaryDirection = (int)(calculatedSize.h / (itemSize.h + gridSpacing.h));
	}
	else if (orientation == OrientationVertical)
	{
		if (size.w == SIZE_WRAP_CONTENT)
			throw runtime_error("grid width cannot depend on content");

		itemCountSecondaryDirection = (int)(calculatedSize.w / (itemSize.w + gridSpacing.w));
	}

	if (itemCountSecondaryDirection <= 0)
		itemCountSecondaryDirection = 1;

	// Calculate column and row sizes
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children.at(i);

		int curColumn;
		int curRow;

		int topContentInset = 0;
		int leftContentInset = 0;

		if (orientation == OrientationHorizontal)
		{
			curColumn = i / itemCountSecondaryDirection;
			curRow = i % itemCountSecondaryDirection;
			leftContentInset = leadingContentInset;
		}
		else
		{
			curColumn = i % itemCountSecondaryDirection;
			curRow = i / itemCountSecondaryDirection;
			topContentInset = leadingContentInset;
		}

		v->CalculateLayout(itemSize);
		v->SetRelativePosition(Position(curColumn * (itemSize.w + gridSpacing.w) + leftContentInset, curRow * (itemSize.h + gridSpacing.h) + topContentInset));
	}
}

View* GridView::Copy()
{
	GridView* view = new GridView();

	CopyBase(view);
	view->SetOrientation(orientation);
	view->SetGridSpacing(gridSpacing);
	view->SetItemSize(itemSize);

	return view;
}

Orientation GridView::GetOrientation()
{
	return orientation;
}

void GridView::SetOrientation(Orientation orientation)
{
	this->orientation = orientation;
}

Size GridView::GetGridSpacing()
{
	return gridSpacing;
}

void GridView::SetGridSpacing(Size gridSpacing)
{
	this->gridSpacing = gridSpacing;
}

Size GridView::GetItemSize()
{
	return itemSize;
}

void GridView::SetItemSize(Size itemSize)
{
	this->itemSize = itemSize;
}

void GridView::SetContentInset(int leading, int trailing)
{
	leadingContentInset = leading;
	trailingContentInset = trailing;
}

bool GridView::SetProperty(string name, string value)
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
	else if (name == "horizontalSpacing")
	{
		stringstream ss(value);
		if ((ss >> gridSpacing.w).fail() || !(ss >> std::ws).eof())
		{
			throw runtime_error("could not parse horizontal spacing");
		}

		return true;
	}
	else if (name == "verticalSpacing")
	{
		stringstream ss(value);
		if ((ss >> gridSpacing.h).fail() || !(ss >> std::ws).eof())
		{
			throw runtime_error("could not parse vertical spacing");
		}

		return true;
	}
	else if (name == "itemWidth")
	{
		stringstream ss(value);
		if ((ss >> itemSize.w).fail() || !(ss >> std::ws).eof())
		{
			throw runtime_error("could not parse item width");
		}

		return true;
	}
	else if (name == "itemHeight")
	{
		stringstream ss(value);
		if ((ss >> itemSize.h).fail() || !(ss >> std::ws).eof())
		{
			throw runtime_error("could not parse item height");
		}

		return true;
	}
	else if (name == "contentInset")
	{
		stringstream ss(value);
		if ((ss >> leadingContentInset).fail())
			throw runtime_error("could not parse contentInset");

		if ((ss >> std::ws).eof())
		{
			// Only one value means we use that value for both leading and trailing
			trailingContentInset = leadingContentInset;
		}
		else
		{
			if ((ss >> trailingContentInset).fail() || !(ss >> std::ws).eof())
				throw runtime_error("could not parse contentInset");
		}

		return true;
	}

	return false;
}

void GridView::GetRowsAndColumns(int& outRows, int& outColumns)
{
	int itemsInLength = children.size() / itemCountSecondaryDirection;

	if (children.size() % itemCountSecondaryDirection != 0)
		itemsInLength++;

	if (orientation == OrientationHorizontal)
	{
		outRows = itemCountSecondaryDirection;
		outColumns = itemsInLength;
	}
	else
	{
		outRows = itemsInLength;
		outColumns = itemCountSecondaryDirection;
	}
}

Position GridView::GetPositionForItemIndex(int elementNo)
{
	Position position;
	position.x = -1;
	position.y = -1;

	if (elementNo < 0 || elementNo >= children.size())
		return position;

	int posX, posY;
	if (orientation == OrientationHorizontal)
	{
		posX = elementNo / itemCountSecondaryDirection;
		posY = elementNo % itemCountSecondaryDirection;
	}
	else
	{
		posX = elementNo % itemCountSecondaryDirection;
		posY = elementNo / itemCountSecondaryDirection;
	}

	position.x = posX;
	position.y = posY;

	return position;
}

int GridView::GetItemIndexForPosition(Position position)
{
	int rows, columns;
	GetRowsAndColumns(rows, columns);

	if (position.x < 0 || position.x >= columns)
		return -1;

	if (position.y < 0 || position.y >= rows)
		return -1;

	if (orientation == OrientationHorizontal)
		return position.x * rows + position.y;
	else
		return position.y * columns + position.x;
}

View* GridView::GetSelectedItem()
{
	return children[selectedIndex];
}

View* GridView::SelectNext(Direction direction)
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

	int rows;
	int columns;
	GetRowsAndColumns(rows, columns);
	Position selectedPosition = GetPositionForItemIndex(selectedIndex);

	if (selectedPosition.x + dx < 0 || selectedPosition.x + dx >= columns)
		return NULL;

	if (selectedPosition.y + dy < 0 || selectedPosition.y + dy >= rows)
		return NULL;

	int origIndex = GetItemIndexForPosition(selectedPosition);
	selectedPosition.x += dx;
	selectedPosition.y += dy;
	int index = GetItemIndexForPosition(selectedPosition);

	// if there is no element at the new index
	if (index >= children.size())
		index = children.size() - 1;

	if (origIndex != index)
	{
		// A new selection was made
		SelectByIndex(index);
	}

	return children[index];
}

bool GridView::Select(Position position)
{
	return SelectByIndex(GetItemIndexForPosition(position));
}

void GridView::OnSelectionChanged()
{
	ScrollTo(children[selectedIndex]);
}
