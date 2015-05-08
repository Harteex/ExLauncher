#include "ScrollView.h"
#include <algorithm>

using namespace std;

ScrollView::ScrollView()
{
	scrollOffset = 0;
	scrollToTargetOffset = 0;
}

void ScrollView::DrawChildren(Position offset, Rectangle viewBounds)
{
	//offset.x += scrollOffset; // FIXME check scroll direction
	offset.y += scrollOffset;

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Draw(offset, viewBounds);
	}
}

void ScrollView::OnUpdate()
{
	if (scrollOffset == scrollToTargetOffset)
		return;

	int distanceToScroll = (scrollToTargetOffset - scrollOffset) / 4;
	if (distanceToScroll == 0)
	{
		if (scrollOffset < scrollToTargetOffset)
			distanceToScroll = 1;
		else
			distanceToScroll = -1;
	}

	scrollOffset += distanceToScroll;
}

void ScrollView::ScrollTo(int offset)
{
	scrollToTargetOffset = offset;
}

// TODO make sure to handle any layoutmargin, margin, padding or similar, and special itemMargin for the new gridview
void ScrollView::ScrollTo(View* child)
{
	int top = child->GetAbsolutePosition().y - child->GetLayoutMargin().top - scrollOffset;
	int bottom = child->GetAbsolutePosition().y + child->GetCalculatedSize().h + child->GetLayoutMargin().bottom - scrollOffset;

	// Determine direction to scroll
	if (top < absolutePosition.y)
	{
		// Scroll upwards

		ScrollTo(child->GetAbsolutePosition().y - absolutePosition.y - child->GetLayoutMargin().top);
	}
	else if (bottom > calculatedSize.h)
	{
		// Scroll downwards

		ScrollTo(child->GetAbsolutePosition().y + child->GetCalculatedSize().h + child->GetLayoutMargin().bottom - calculatedSize.h);
	}
}
