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
	Position childOffset;
	childOffset.x = 0;
	childOffset.y = 0;

	contentSize.w = 0;
	contentSize.h = 0;

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
}

View* LinearPanel::Copy()
{
	LinearPanel* panel = new LinearPanel();

	panel->SetSize(size);
	panel->SetRelativePosition(relativePosition);
	panel->SetOrientation(orientation);

	for (View* view : children)
	{
		panel->AddChildView(view->Copy());
	}

	return panel;
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
			throw exception("invalid orientation");

		return true;
	}

	return false;
}
