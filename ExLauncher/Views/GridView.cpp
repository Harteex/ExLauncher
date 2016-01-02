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
}

GridView::~GridView()
{
}

bool GridView::OnInitialize()
{
	ScrollView::SetOrientation(orientation);
	return true;
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

		if (orientation == OrientationHorizontal)
		{
			curColumn = i / itemCountSecondaryDirection;
			curRow = i % itemCountSecondaryDirection;
		}
		else
		{
			curColumn = i % itemCountSecondaryDirection;
			curRow = i / itemCountSecondaryDirection;
		}

		v->CalculateLayout(itemSize);
		v->SetRelativePosition(Position(curColumn * (itemSize.w + gridSpacing.w), curRow * (itemSize.h + gridSpacing.h)));
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
