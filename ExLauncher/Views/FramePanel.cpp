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

bool FramePanel::Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer)
{
	isInitialized = true;
	return true;
}

void FramePanel::Update()
{
}

void FramePanel::Draw(SDL_Renderer* renderer)
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

void FramePanel::OnLayoutChange()
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
		v->SetRelativePosition(Position(childMargin.left, childMargin.top));
		Size sizeAreaForChild;
		sizeAreaForChild.w = size.w == SIZE_WRAP_CONTENT ? -1 : calculatedSize.w - (childMargin.left + childMargin.right);
		sizeAreaForChild.h = size.h == SIZE_WRAP_CONTENT ? -1 : calculatedSize.h - (childMargin.top + childMargin.bottom);
		Size childSize = v->CalculateLayout(Position(0, 0), sizeAreaForChild);
		Size childSizeIncMargins = childSize + Size(childMargin.left + childMargin.right, childMargin.top + childMargin.bottom);

		contentSize.w = max(contentSize.w, childSizeIncMargins.w);
		contentSize.h = max(contentSize.h, childSizeIncMargins.h);
	}
}

View* FramePanel::Copy()
{
	FramePanel* panel = new FramePanel();

	panel->SetSize(size);
	panel->SetRelativePosition(relativePosition);

	for (View* view : children)
	{
		panel->AddChildView(view->Copy());
	}

	return panel;
}

bool FramePanel::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	return false;
}
