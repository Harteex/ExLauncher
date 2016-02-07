/*
Copyright 2016 Andreas Bjerkeholt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "ScrollView.h"
#include <algorithm>

using namespace std;

ScrollView::ScrollView()
{
	scrollOffset = 0;
	scrollToTargetOffset = 0;
	orientation = OrientationHorizontal;
}

void ScrollView::DrawChildren(Position offset, Rectangle viewBounds)
{
	if (orientation == OrientationHorizontal)
		offset.x -= scrollOffset;
	else
		offset.y -= scrollOffset;

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
	if (orientation == OrientationHorizontal)
	{
		int left = child->GetAbsolutePosition().x - child->GetLayoutMargin().left - scrollOffset;
		int right = child->GetAbsolutePosition().x + child->GetCalculatedSize().w + child->GetLayoutMargin().right - scrollOffset;

		// Determine direction to scroll
		if (left < absolutePosition.x)
		{
			// Scroll upwards

			ScrollTo(child->GetAbsolutePosition().x - absolutePosition.x - child->GetLayoutMargin().left);
		}
		else if (right > calculatedSize.w)
		{
			// Scroll downwards

			ScrollTo(child->GetAbsolutePosition().x + child->GetCalculatedSize().w + child->GetLayoutMargin().right - calculatedSize.w);
		}
	}
	else
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
}

void ScrollView::SetOrientation(Orientation orientation)
{
	this->orientation = orientation;
}
