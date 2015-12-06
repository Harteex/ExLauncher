#include "LinearPanel.h"
#include <algorithm>

using namespace std;

LinearPanel::LinearPanel()
{
	contentSize.w = 0;
	contentSize.h = 0;
	orientation = OrientationHorizontal;
}

LinearPanel::~LinearPanel()
{
}

bool LinearPanel::OnInitialize()
{
	ScrollView::SetOrientation(orientation);
	return true;
}

void LinearPanel::OnLayoutChange()
{
	Position childOffset = Position(0, 0);
	contentSize = Size(0, 0);

	Size* childSizes = new Size[children.size()];

	if ((size.w == SIZE_FILL_PARENT || size.w > 0) && orientation == OrientationVertical)
		contentSize.w = calculatedSize.w;
	if ((size.h == SIZE_FILL_PARENT || size.h > 0) && orientation == OrientationHorizontal)
		contentSize.h = calculatedSize.h;

	int numberOfViewsFillParent = 0;
	int sizeTakenBySetSizes = 0;
	int sizeAvailableForFillingPerChild = 0;

	// Step 1: Calculate those sizes we can calculate
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children.at(i);

		if (orientation == OrientationHorizontal)
		{
			if (v->GetSize().w == SIZE_FILL_PARENT)
			{
				if (size.w == SIZE_WRAP_CONTENT)
					throw runtime_error("cannot calculate layout size");

				numberOfViewsFillParent++;
				childSizes[i] = Size(0, 0);
				continue;
			}
		}
		else
		{
			if (v->GetSize().h == SIZE_FILL_PARENT)
			{
				if (size.h == SIZE_WRAP_CONTENT)
					throw runtime_error("cannot calculate layout size");

				numberOfViewsFillParent++;
				childSizes[i] = Size(0, 0);
				continue;
			}
		}

		Box childMargin = v->GetLayoutMargin();

		Size sizeAvailableForChild;
		sizeAvailableForChild.w = orientation == OrientationHorizontal ? -1 : calculatedSize.w;
		sizeAvailableForChild.h = orientation == OrientationVertical ? -1 : calculatedSize.h;
		Size childSize = v->CalculateLayout(sizeAvailableForChild);
		Size childSizeIncMargins = childSize + Size(childMargin.left + childMargin.right, childMargin.top + childMargin.bottom);
		childSizes[i] = childSizeIncMargins;

		if (orientation == OrientationHorizontal)
			sizeTakenBySetSizes += childSizes[i].w;
		else
			sizeTakenBySetSizes += childSizes[i].h;
	}

	// Step 2: Calculate size for views filling parent
	if (numberOfViewsFillParent > 0)
	{
		int totalSize = 0;
		if (orientation == OrientationHorizontal)
			totalSize = calculatedSize.w;
		else
			totalSize = calculatedSize.h;

		sizeAvailableForFillingPerChild = (totalSize - sizeTakenBySetSizes) / numberOfViewsFillParent;
	}

	// Step 3: Calculate those children with fill parent
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children.at(i);

		if (orientation == OrientationHorizontal)
		{
			if (v->GetSize().w != SIZE_FILL_PARENT)
				continue;
		}
		else
		{
			if (v->GetSize().h != SIZE_FILL_PARENT)
				continue;
		}

		Box childMargin = v->GetLayoutMargin();

		Size sizeAvailableForChild = calculatedSize;
		if (orientation == OrientationHorizontal)
		{
			sizeAvailableForChild.w = sizeAvailableForFillingPerChild - childMargin.left - childMargin.right;

			// Also fix top / down margin
			if (calculatedSize.h > 0)
				sizeAvailableForChild.h = max(calculatedSize.h - childMargin.top - childMargin.bottom, 0);
		}
		else
		{
			sizeAvailableForChild.h = sizeAvailableForFillingPerChild - childMargin.top - childMargin.bottom;

			// Also fix left / right margin
			if (calculatedSize.w > 0)
				sizeAvailableForChild.w = max(calculatedSize.w - childMargin.left - childMargin.right, 0);
		}

		Size childSize = v->CalculateLayout(sizeAvailableForChild);
		Size childSizeIncMargins = childSize + Size(childMargin.left + childMargin.right, childMargin.top + childMargin.bottom);
		childSizes[i] = childSizeIncMargins;
	}

	// Step 4: Position children
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children.at(i);

		Box childMargin = v->GetLayoutMargin();
		v->SetRelativePosition(childOffset + Position(childMargin.left, childMargin.top));

		if (orientation == OrientationHorizontal)
		{
			childOffset.x += childSizes[i].w;
			contentSize.w += childSizes[i].w;
			if (size.h == SIZE_WRAP_CONTENT)
				contentSize.h = max(contentSize.h, childSizes[i].h);
		}
		else
		{
			childOffset.y += childSizes[i].h;
			contentSize.h += childSizes[i].h;
			if (size.w == SIZE_WRAP_CONTENT)
				contentSize.w = max(contentSize.w, childSizes[i].w);
		}
	}

	// Step 5: Adjust child positions for gravity
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children.at(i);

		Size containerSize;
		if (orientation == OrientationHorizontal)
		{
			containerSize.w = childSizes[i].w;
			containerSize.h = contentSize.h;
		}
		else
		{
			containerSize.w = contentSize.w;
			containerSize.h = childSizes[i].h;
		}
		
		Position gravityOffset = GetGravityOffset(childSizes[i], containerSize, children[i]->GetLayoutGravity());
		v->SetRelativePosition(v->GetRelativePosition() + gravityOffset);
	}

	delete[] childSizes;
}

View* LinearPanel::Copy()
{
	LinearPanel* view = new LinearPanel();

	CopyBase(view);
	view->SetOrientation(orientation);

	return view;
}

Orientation LinearPanel::GetOrientation()
{
	return orientation;
}

void LinearPanel::SetOrientation(Orientation orientation)
{
	this->orientation = orientation;
}

bool LinearPanel::SetProperty(string name, string value)
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

	return false;
}

Position LinearPanel::GetPositionForItemIndex(int elementNo)
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

int LinearPanel::GetItemIndexForPosition(Position position)
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

View* LinearPanel::GetSelectedItem()
{
	return children[selectedIndex];
}

View* LinearPanel::SelectNext(Direction direction)
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

View* LinearPanel::Select(Position position)
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

void LinearPanel::OnSelectionChanged()
{
	ScrollTo(children[selectedIndex]);
}
