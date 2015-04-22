#include "GridView.h"

using namespace std;

GridView::GridView()
{
	contentSize.w = 0;
	contentSize.h = 0;
	selectedPosition = -1;
	itemCountSecondaryDirection = 1;
	orientation = OrientationHorizontal;

	itemMargin = 0;
}

GridView::~GridView()
{
}

bool GridView::Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer)
{
	isInitialized = true;
	return true;
}

void GridView::Update()
{
}

void GridView::Draw(SDL_Renderer* renderer)
{
	if (calculatedSize.w == 0 || calculatedSize.h == 0)
		return;

	if (contentSize.w == 0 || contentSize.h == 0)
		return;

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Draw(renderer);
	}
}

void GridView::OnLayoutChange()
{
	if (itemCountSecondaryDirection <= 0)
		throw exception("GridView: itemCountSecondaryDirection must be greater than zero");

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
		Size childSize = v->CalculateLayout(Position(0, 0), Size(-1, -1));
		Size childSizeIncMargins = childSize + Size(childMargin.left + childMargin.right, childMargin.top + childMargin.bottom);

		if (childSizeIncMargins.w > columnSizes[curColumn])
			columnSizes[curColumn] = childSizeIncMargins.w;

		if (childSizeIncMargins.h > rowSizes[curRow])
			rowSizes[curRow] = childSizeIncMargins.h;
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

		Box childMargin = v->GetLayoutMargin();
		v->SetRelativePosition(columnPositions[curColumn] + childMargin.left, rowPositions[curRow] + childMargin.top);
	}
}

View* GridView::Copy()
{
	GridView* gridView = new GridView();

	gridView->SetSize(size);
	gridView->SetRelativePosition(relativePosition);
	gridView->SetItemCountSecondaryDirection(itemCountSecondaryDirection);
	gridView->SetOrientation(orientation);

	for (View* view : children)
	{
		gridView->AddChildView(view->Copy());
	}

	return gridView;
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
			throw exception("invalid orientation");

		return true;
	}
	else if (name == "itemCountSecondaryDirection")
	{
		int itemCountSecondaryDirectionIntValue = atoi(value.c_str());
		if (itemCountSecondaryDirectionIntValue <= 0)
			throw exception("invalid value for itemCountSecondaryDirection");
		SetItemCountSecondaryDirection(itemCountSecondaryDirectionIntValue);

		return true;
	}

	return false;
}
