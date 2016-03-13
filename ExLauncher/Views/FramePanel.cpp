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

#include "FramePanel.h"
#include <algorithm>
#include "../ViewSystem/LayoutHelper.h"

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

		if (v->GetSize().w == SIZE_FILL_PARENT && size.w == SIZE_WRAP_CONTENT ||
			v->GetSize().h == SIZE_FILL_PARENT && size.h == SIZE_WRAP_CONTENT)
			throw runtime_error("cannot calculate layout size");

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

	Size containerSize = calculatedSize;
	if (size.w == SIZE_WRAP_CONTENT)
		containerSize.w = contentSize.w;
	if (size.h == SIZE_WRAP_CONTENT)
		containerSize.h = contentSize.h;

	// Adjust child positions for gravity
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children.at(i);

		Position gravityOffset = LayoutHelper::GetGravityOffset(childSizes[i], containerSize, children[i]->GetLayoutGravity());
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
