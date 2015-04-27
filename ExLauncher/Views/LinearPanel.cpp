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

bool LinearPanel::Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer)
{
	isInitialized = true;
	return true;
}

void LinearPanel::Update()
{
}

void LinearPanel::Draw(SDL_Renderer* renderer)
{
	if (!IsVisible())
		return;

	if (calculatedSize.w == 0 || calculatedSize.h == 0)
		return;

	if (contentSize.w == 0 || contentSize.h == 0)
		return;

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Draw(renderer);
	}
}

void LinearPanel::OnLayoutChange()
{
	Position childOffset = Position(0, 0);
	contentSize = Size(0, 0);

	Size* childSizes = new Size[children.size()];

	// Position children
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children.at(i);

		Box childMargin = v->GetLayoutMargin();
		Size sizeAreaForChild;
		sizeAreaForChild.w = size.w == SIZE_WRAP_CONTENT ? -1 : calculatedSize.w - (childMargin.left + childMargin.right);
		sizeAreaForChild.h = size.h == SIZE_WRAP_CONTENT ? -1 : calculatedSize.h - (childMargin.top + childMargin.bottom);
		Size childSize = v->CalculateLayout(childOffset, sizeAreaForChild);
		Size childSizeIncMargins = childSize + Size(childMargin.left + childMargin.right, childMargin.top + childMargin.bottom);
		childSizes[i] = childSizeIncMargins;

		v->SetRelativePosition(childOffset + Position(childMargin.left, childMargin.top));

		if (orientation == OrientationHorizontal)
		{
			childOffset.x += childSizeIncMargins.w;
			contentSize.w += childSizeIncMargins.w;
			contentSize.h = max(contentSize.h, childSizeIncMargins.h);
		}
		else
		{
			childOffset.y += childSizeIncMargins.h;
			contentSize.w = max(contentSize.w, childSizeIncMargins.w);
			contentSize.h += childSizeIncMargins.h;
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
