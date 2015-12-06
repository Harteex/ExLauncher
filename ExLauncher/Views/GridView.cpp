#include "GridView.h"

using namespace std;

GridView::GridView()
{
	contentSize.w = 0;
	contentSize.h = 0;
	itemCountSecondaryDirection = 1;
	orientation = OrientationHorizontal;

	itemMargin = 0;
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
	if (itemCountSecondaryDirection <= 0)
		throw runtime_error("GridView: itemCountSecondaryDirection must be greater than zero");

	int itemsInWidth = 0;
	int itemsInHeight = 0;

	if (orientation == OrientationHorizontal)
	{
		itemsInWidth = children.size() / itemCountSecondaryDirection;
		itemsInHeight = itemCountSecondaryDirection;

		if (children.size() % itemCountSecondaryDirection != 0)
			itemsInWidth++;
	}
	else
	{
		itemsInWidth = itemCountSecondaryDirection;
		itemsInHeight = children.size() / itemCountSecondaryDirection;

		if (children.size() % itemCountSecondaryDirection != 0)
			itemsInHeight++;
	}

	int* columnSizes = new int[itemsInWidth];
	int* rowSizes = new int[itemsInHeight];

	int* columnPositions = new int[itemsInWidth];
	int* rowPositions = new int[itemsInHeight];

	Size* childSizes = new Size[children.size()];

	memset(columnSizes, 0, itemsInWidth * sizeof(int));
	memset(rowSizes, 0, itemsInHeight * sizeof(int));

	memset(columnPositions, 0, itemsInWidth * sizeof(int));
	memset(rowPositions, 0, itemsInHeight * sizeof(int));

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

		Box childMargin = v->GetLayoutMargin();
		Size childSize = v->CalculateLayout(Size(-1, -1));
		Size childSizeIncMargins = childSize + Size(childMargin.left + childMargin.right, childMargin.top + childMargin.bottom);

		if (childSizeIncMargins.w > columnSizes[curColumn])
			columnSizes[curColumn] = childSizeIncMargins.w;

		if (childSizeIncMargins.h > rowSizes[curRow])
			rowSizes[curRow] = childSizeIncMargins.h;

		childSizes[i] = childSizeIncMargins;
	}

	// Calculate positions for columns and rows
	for (int i = 0; i < itemsInWidth; i++)
	{
		if (i == 0)
			columnPositions[i] = 0;
		else
			columnPositions[i] = columnPositions[i - 1] + columnSizes[i - 1] + itemMargin;
	}

	for (int i = 0; i < itemsInHeight; i++)
	{
		if (i == 0)
			rowPositions[i] = 0;
		else
			rowPositions[i] = rowPositions[i - 1] + rowSizes[i - 1] + itemMargin;
	}

	// Set content area size
	contentSize.w = columnPositions[itemsInWidth - 1] + columnSizes[itemsInWidth - 1];
	contentSize.h = rowPositions[itemsInHeight - 1] + rowSizes[itemsInHeight - 1];

	// Position children
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

		Size curContainer = Size(columnSizes[curColumn], rowSizes[curRow]);
		Position gravityOffset = GetGravityOffset(childSizes[i], curContainer, children[i]->GetLayoutGravity());

		Box childMargin = v->GetLayoutMargin();
		v->SetRelativePosition(columnPositions[curColumn] + childMargin.left + gravityOffset.x, rowPositions[curRow] + childMargin.top + gravityOffset.y);
	}

	delete[] columnSizes;
	delete[] rowSizes;
	delete[] columnPositions;
	delete[] rowPositions;
	delete[] childSizes;
}

View* GridView::Copy()
{
	GridView* view = new GridView();

	CopyBase(view);
	view->SetItemCountSecondaryDirection(itemCountSecondaryDirection);
	view->SetOrientation(orientation);

	return view;
}

int GridView::GetItemCountSecondaryDirection()
{
	return itemCountSecondaryDirection;
}

void GridView::SetItemCountSecondaryDirection(int count)
{
	itemCountSecondaryDirection = count;
}

Orientation GridView::GetOrientation()
{
	return orientation;
}

void GridView::SetOrientation(Orientation orientation)
{
	this->orientation = orientation;
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
	else if (name == "itemCountSecondaryDirection")
	{
		int itemCountSecondaryDirectionIntValue = atoi(value.c_str());
		if (itemCountSecondaryDirectionIntValue <= 0)
			throw runtime_error("invalid value for itemCountSecondaryDirection");
		SetItemCountSecondaryDirection(itemCountSecondaryDirectionIntValue);

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
