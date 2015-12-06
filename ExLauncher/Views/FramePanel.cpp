#include "FramePanel.h"
#include <algorithm>

using namespace std;

FramePanel::FramePanel()
{
	contentSize.w = 0;
	contentSize.h = 0;
}

FramePanel::~FramePanel()
{
}

void FramePanel::OnLayoutChange()
{
	Position childOffset = Position(0, 0);
	contentSize = Size(0, 0);

	Size* childSizes = new Size[children.size()];

	if (size.w == SIZE_FILL_PARENT || size.w > 0)
		contentSize.w = calculatedSize.w;
	if (size.h == SIZE_FILL_PARENT || size.h > 0)
		contentSize.h = calculatedSize.h;

	// Position children
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children.at(i);

		Box childMargin = v->GetLayoutMargin();
		v->SetRelativePosition(Position(childMargin.left, childMargin.top));
		Size sizeAreaForChild;
		sizeAreaForChild.w = max(size.w == SIZE_WRAP_CONTENT ? -1 : calculatedSize.w - (childMargin.left + childMargin.right), 0);
		sizeAreaForChild.h = max(size.h == SIZE_WRAP_CONTENT ? -1 : calculatedSize.h - (childMargin.top + childMargin.bottom), 0);
		Size childSize = v->CalculateLayout(sizeAreaForChild);
		Size childSizeIncMargins = childSize + Size(childMargin.left + childMargin.right, childMargin.top + childMargin.bottom);
		childSizes[i] = childSizeIncMargins;

		contentSize.w = max(contentSize.w, childSizeIncMargins.w);
		contentSize.h = max(contentSize.h, childSizeIncMargins.h);
	}

	// Adjust child positions for gravity
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children.at(i);

		Position gravityOffset = GetGravityOffset(childSizes[i], contentSize, children[i]->GetLayoutGravity());
		v->SetRelativePosition(v->GetRelativePosition() + gravityOffset);
	}

	delete[] childSizes;
}

View* FramePanel::Copy()
{
	FramePanel* view = new FramePanel();

	CopyBase(view);

	return view;
}

bool FramePanel::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	return false;
}
