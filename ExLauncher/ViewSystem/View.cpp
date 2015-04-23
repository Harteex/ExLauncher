#include "View.h"
#include "../utils.h"
#include <algorithm>

using namespace std;

View::View()
{
	isInitialized = false;
	id = "";
	visible = true;
	relativePosition = Position(0, 0);
	absolutePosition = Position(0, 0);
	size = Size(0, 0);
	calculatedSize = Size(0, 0);
	contentSize = Size(-1, -1);
	layoutMargin = Box();
	itemTemplate = NULL;
}

string View::GetId()
{
	return id;
}

void View::SetId(string id)
{
	this->id = id;
}

vector<string> View::GetTags()
{
	return tags;
}

bool View::InitializeAll(ResourceManager* resourceManager, SDL_Renderer* renderer)
{
	bool result = true;
	result = Initialize(resourceManager, renderer);
	if (!result)
		return false;

	for (View* view : children)
	{
		result = result && view->InitializeAll(resourceManager, renderer);
	}

	return result;
}

Position View::GetRelativePosition()
{
	return relativePosition;
}

void View::SetRelativePosition(Position position)
{
	this->relativePosition = position;
}

void View::SetRelativePosition(int x, int y)
{
	relativePosition.x = x;
	relativePosition.y = y;
}

Position View::GetAbsolutePosition()
{
	return absolutePosition;
}

Size View::GetSize()
{
	return size;
}

void View::SetSize(Size size)
{
	this->size = size;
}

void View::SetSize(int width, int height)
{
	size.w = width;
	size.h = height;
}

Size View::GetContentSize()
{
	return contentSize;
}

Box View::GetLayoutMargin()
{
	return layoutMargin;
}

Size View::CalculateLayout(Position offsetInCurView, Size parentSize)
{
	if (size.w == SIZE_MATCH_PARENT && parentSize.w < 0 || size.h == SIZE_MATCH_PARENT && parentSize.h < 0)
		throw runtime_error("cannot calculate layout size");

	if (size.w == SIZE_MATCH_PARENT)
		calculatedSize.w = max(parentSize.w - offsetInCurView.x, 0);
	else if (size.w > 0)
		calculatedSize.w = size.w;

	if (size.h == SIZE_MATCH_PARENT)
		calculatedSize.h = max(parentSize.h - offsetInCurView.y, 0);
	else if (size.h > 0)
		calculatedSize.h = size.h;

	OnLayoutChange();

	if (size.w == SIZE_WRAP_CONTENT)
		calculatedSize.w = contentSize.w;

	if (size.h == SIZE_WRAP_CONTENT)
		calculatedSize.h = contentSize.h;

	return calculatedSize;
}

void View::OnLayoutChange()
{
}

void View::RecalculateLayout()
{
	if (parent != NULL && (parent->GetSize().w == SIZE_WRAP_CONTENT || parent->GetSize().h == SIZE_WRAP_CONTENT))
	{
		parent->RecalculateLayout();
	}
	else
	{
		OnLayoutChange();

		if (size.w == SIZE_WRAP_CONTENT)
			calculatedSize.w = contentSize.w;

		if (size.h == SIZE_WRAP_CONTENT)
			calculatedSize.h = contentSize.h;

		CalculateAbsolutePosition(parent != NULL ? parent->GetAbsolutePosition() : Position(0, 0));
	}
}

void View::CalculateAbsolutePosition(Position parentPosition)
{
	absolutePosition = relativePosition + parentPosition;

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->CalculateAbsolutePosition(absolutePosition);
	}
}

View* View::GetParentView()
{
	return parent;
}

void View::SetParentView(View* parent)
{
	this->parent = parent;
}

View* View::GetChildViewById(string id)
{
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i]->GetId() == id)
			return children[i];
	}

	for (int i = 0; i < children.size(); i++)
	{
		View* v = children[i]->GetChildViewById(id);

		if (v != NULL)
			return v;
	}

	return NULL;
}

View* View::GetFirstChildViewByTag(string tag)
{
	for (int i = 0; i < children.size(); i++)
	{
		vector<string> tags = children[i]->GetTags();

		for (int j = 0; j < tags.size(); j++)
		{
			if (tags[j] == tag)
				return children[i];
		}
	}

	for (int i = 0; i < children.size(); i++)
	{
		View* v = children[i]->GetFirstChildViewByTag(tag);

		if (v != NULL)
			return v;
	}

	return NULL;
}

void View::AddChildView(View* view)
{
	view->SetParentView(this);
	children.push_back(view);
}

int View::GetNumberOfChildren()
{
	return children.size();
}

View* View::GetChildView(int i)
{
	return children.at(i);
}

bool View::IsVisible()
{
	return visible;
}

void View::SetVisible(bool visible)
{
	this->visible = visible;
}

bool View::SetProperty(string name, string value)
{
	if (name == "id")
	{
		this->id = value;
		return true;
	}
	else if (name == "tag")
	{
		tags = split(value, '|');
		return true;
	}
	else if (name == "width")
	{
		if (value == "wrap_content")
			size.w = SIZE_WRAP_CONTENT;
		else if (value == "match_parent")
			size.w = SIZE_MATCH_PARENT;
		else
			size.w = atoi(value.c_str());

		return true;
	}
	else if (name == "height")
	{
		if (value == "wrap_content")
			size.h = SIZE_WRAP_CONTENT;
		else if (value == "match_parent")
			size.h = SIZE_MATCH_PARENT;
		else
			size.h = atoi(value.c_str());

		return true;
	}
	else if (name == "layoutMargin")
	{
		int n = atoi(value.c_str());
		layoutMargin = Box(n, n, n, n);
		return true;
	}

	return false;
}

View* View::GetItemTemplate()
{
	return itemTemplate;
}

void View::SetItemTemplate(View* itemTemplate)
{
	this->itemTemplate = itemTemplate;
}

void View::FillData(map<string, string>& data)
{
}
