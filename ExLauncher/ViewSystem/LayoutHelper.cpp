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

#include "LayoutHelper.h"
#include <algorithm>

using namespace std;

void LayoutHelper::LayoutChildInContainer(Size container, View* child)
{
	Box childMargin = child->GetLayoutMargin();
	child->SetRelativePosition(Position(childMargin.left, childMargin.top));

	Size sizeAreaForChild;
	sizeAreaForChild.w = max(container.w - (childMargin.left + childMargin.right), 0);
	sizeAreaForChild.h = max(container.h - (childMargin.top + childMargin.bottom), 0);
	Size childSize = child->CalculateLayout(sizeAreaForChild);
	Size childSizeIncMargins = childSize + Size(childMargin.left + childMargin.right, childMargin.top + childMargin.bottom);

	// Adjust for gravity
	Position gravityOffset = GetGravityOffset(childSizeIncMargins, container, child->GetLayoutGravity());
	child->SetRelativePosition(child->GetRelativePosition() + gravityOffset);
}

Position LayoutHelper::GetGravityOffset(Size childSize, Size containerSize, int childLayoutGravity)
{
	Size freeSpace = containerSize - childSize;

	int gravityOffsetW = 0;
	int gravityOffsetH = 0;

	if ((childLayoutGravity & GRAVITY_HCENTER) > 0)
		gravityOffsetW = freeSpace.w / 2;
	else if ((childLayoutGravity & GRAVITY_RIGHT) > 0)
		gravityOffsetW = freeSpace.w;

	if ((childLayoutGravity & GRAVITY_VCENTER) > 0)
		gravityOffsetH = freeSpace.h / 2;
	else if ((childLayoutGravity & GRAVITY_BOTTOM) > 0)
		gravityOffsetH = freeSpace.h;

	return Position(gravityOffsetW, gravityOffsetH);
}
