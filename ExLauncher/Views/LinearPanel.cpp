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

	if ((size.w == SIZE_MATCH_PARENT || size.w > 0) && orientation == OrientationVertical)
		contentSize.w = calculatedSize.w;
	if ((size.h == SIZE_MATCH_PARENT || size.h > 0) && orientation == OrientationHorizontal)
		contentSize.h = calculatedSize.h;

	// Position children
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children.at(i);

		Box childMargin = v->GetLayoutMargin();
		Size sizeAreaForChild;
		sizeAreaForChild.w = max(size.w == SIZE_WRAP_CONTENT ? -1 : calculatedSize.w - (childMargin.left + childMargin.right), 0);
		sizeAreaForChild.h = max(size.h == SIZE_WRAP_CONTENT ? -1 : calculatedSize.h - (childMargin.top + childMargin.bottom), 0);
		Size childSize = v->CalculateLayout(childOffset, sizeAreaForChild);
		Size childSizeIncMargins = childSize + Size(childMargin.left + childMargin.right, childMargin.top + childMargin.bottom);
		childSizes[i] = childSizeIncMargins;

		v->SetRelativePosition(childOffset + Position(childMargin.left, childMargin.top));

		if (orientation == OrientationHorizontal)
		{
			childOffset.x += childSizeIncMargins.w;
			contentSize.w += childSizeIncMargins.w;
			if (size.h == SIZE_WRAP_CONTENT)
				contentSize.h = max(contentSize.h, childSizeIncMargins.h);
		}
		else
		{
			childOffset.y += childSizeIncMargins.h;
			contentSize.h += childSizeIncMargins.h;
			if (size.w == SIZE_WRAP_CONTENT)
				contentSize.w = max(contentSize.w, childSizeIncMargins.w);
		}
	}

	// Adjust child positions for gravity
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

	int origIndex = selectedIndex;
	int index;

	if (orientation == OrientationHorizontal)
	{
		if (origIndex + dx < 0 || origIndex + dx >= children.size())
			return NULL;

		if (dy != 0)
			return NULL;

		index += dx;
	}

	if (orientation == OrientationVertical)
	{
		if (dx != 0)
			return NULL;

		if (origIndex + dy < 0 || origIndex + dy >= children.size())
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
