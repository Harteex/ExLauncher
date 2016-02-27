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
	leadingContentInset = 0;
	trailingContentInset = 0;
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

void ScrollView::ScrollTo(View* child)
{
	if (orientation == OrientationHorizontal)
	{
		int left = child->GetRelativePosition().x - leadingContentInset - scrollOffset;
		int right = child->GetRelativePosition().x + child->GetCalculatedSize().w + trailingContentInset - scrollOffset;

		// Determine direction to scroll
		if (left < 0)
		{
			// Scroll left
			ScrollTo(child->GetRelativePosition().x - leadingContentInset);
		}
		else if (right > calculatedSize.w)
		{
			// Scroll right
			ScrollTo(child->GetRelativePosition().x + child->GetCalculatedSize().w + trailingContentInset - calculatedSize.w);
		}
	}
	else
	{
		int top = child->GetRelativePosition().y - leadingContentInset - scrollOffset;
		int bottom = child->GetRelativePosition().y + child->GetCalculatedSize().h + trailingContentInset - scrollOffset;

		// Determine direction to scroll
		if (top < 0)
		{
			// Scroll upwards
			ScrollTo(child->GetRelativePosition().y - leadingContentInset);
		}
		else if (bottom > calculatedSize.h)
		{
			// Scroll downwards
			ScrollTo(child->GetRelativePosition().y + child->GetCalculatedSize().h + trailingContentInset - calculatedSize.h);
		}
	}
}

void ScrollView::SetOrientation(Orientation orientation)
{
	this->orientation = orientation;
}

void ScrollView::SetContentInset(int leading, int trailing)
{
	leadingContentInset = leading;
	trailingContentInset = trailing;
}
