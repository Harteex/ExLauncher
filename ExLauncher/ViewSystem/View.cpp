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

#include "View.h"
#include "../utils.h"
#include <algorithm>
#include "../global.h"
#include "../Graphics/graphics_utils.h"
#include "../ScreenSystem/Screen.h"
#include "../Theme/ThemeManager.h"

using namespace std;

View::View()
{
	isInitialized = false;
	context = NULL;
	id = "";
	name = "";
	parent = NULL;
	visible = true;
	relativePosition = Position(0, 0);
	absolutePosition = Position(0, 0);
	size = Size(0, 0);
	calculatedSize = Size(0, 0);
	contentSize = Size(-1, -1);
	layoutMargin = Box();
	layoutGravity = 0;
	gravity = 0;
	itemTemplate = NULL;
	background = Color(0, 0, 0, 0);
	action = "";
	actionArgs = vector<string>();
}

View::~View()
{
	for (View* v : children)
		delete v;

	children.clear();
}

string View::GetId()
{
	return id;
}

void View::SetId(string id)
{
	this->id = id;
}

string View::GetName()
{
	return name;
}

void View::SetName(string name)
{
	this->name = name;
}

vector<string> View::GetTags()
{
	return tags;
}

void View::AddTag(std::string tag)
{
	tags.push_back(tag);
}

void View::Initialize(Screen* context)
{
	if (isInitialized)
		return;

	this->context = context;

	OnInitialize();

	isInitialized = true;
}

void View::OnInitialize()
{
}

void View::InitializeAll(Screen* context)
{
	Initialize(context);

	for (View* view : children)
	{
		view->InitializeAll(context);
	}
}

bool View::IsInitialized()
{
	return isInitialized;
}

void View::Update()
{
	OnUpdate();

	for (View* v : children)
	{
		v->Update();
	}
}

void View::OnUpdate()
{
}

void View::Draw(Position offset, Rectangle parentViewBounds)
{
	if (!IsVisible())
		return;

	if (calculatedSize.w == 0 || calculatedSize.h == 0)
		return;

	/*if (contentSize.w == 0 || contentSize.h == 0)
		return;*/

	Rectangle viewBounds = CalculateViewBounds(offset, parentViewBounds);

	if (viewBounds.w == 0 || viewBounds.h == 0)
		return;

	SDL_Rect viewBoundsRectangle;
	viewBoundsRectangle.x = viewBounds.x;
	viewBoundsRectangle.y = viewBounds.y;
	viewBoundsRectangle.w = viewBounds.w;
	viewBoundsRectangle.h = viewBounds.h;

	DrawDebugViewBounds(viewBounds);

	Graphics& graphics = context->GetGraphics();

	graphics.SetClipRect(viewBounds);
	DrawBackground(offset);
	OnDraw(graphics, offset);
	graphics.ClearClipRect();

	DrawChildren(offset, viewBounds);
}

void View::OnDraw(Graphics& graphics, Position offset)
{
}

void View::DrawBackground(Position offset)
{
	if (background.a > 0)
	{
		Graphics& graphics = context->GetGraphics();
		graphics.FillRect(absolutePosition.x + offset.x, absolutePosition.y + offset.y, calculatedSize.w, calculatedSize.h, background);
	}
}

void View::DrawDebugViewBounds(Rectangle viewBounds)
{
	if (debugViewBounds)
	{
		context->GetGraphics().DrawRect(viewBounds, Color(0xff, 0x00, 0x00, 0xff));
	}
}

void View::DrawChildren(Position offset, Rectangle viewBounds)
{
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Draw(offset, viewBounds);
	}
}

Rectangle View::CalculateViewBounds(Position offset, Rectangle parentViewBounds)
{
	Rectangle viewBounds = Rectangle(absolutePosition.x + offset.x, absolutePosition.y + offset.y, calculatedSize.w, calculatedSize.h);

	if (parentViewBounds.w != -1 && parentViewBounds.h != -1)
	{
		viewBounds = ClipRect(viewBounds, parentViewBounds);
	}

	return viewBounds;
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

Size View::GetCalculatedSize()
{
	return calculatedSize;
}

Size View::GetContentSize()
{
	return contentSize;
}

Box View::GetLayoutMargin()
{
	return layoutMargin;
}

void View::SetLayoutMargin(Box margin)
{
	this->layoutMargin = margin;
}

int  View::GetLayoutGravity()
{
	return layoutGravity;
}

void View::SetLayoutGravity(int gravity)
{
	this->layoutGravity = gravity;
}

int  View::GetGravity()
{
	return gravity;
}

void View::SetGravity(int gravity)
{
	this->gravity = gravity;
}

Color View::GetBackground()
{
	return background;
}

void View::SetBackground(Color background)
{
	this->background = background;
}

std::string View::GetAction()
{
	return action;
}

void View::SetAction(std::string action)
{
	this->action = action;
}

vector<string> View::GetActionArgs()
{
	return actionArgs;
}

void View::SetActionArgs(vector<string> actionArgs)
{
	this->actionArgs = actionArgs;
}

Size View::CalculateLayout(Size sizeAvailable)
{
	if (size.w == SIZE_FILL_PARENT && sizeAvailable.w < 0 || size.h == SIZE_FILL_PARENT && sizeAvailable.h < 0)
		throw runtime_error("cannot calculate layout size");

	calculatedSize = Size(0, 0);

	if (size.w == SIZE_FILL_PARENT)
		calculatedSize.w = max(sizeAvailable.w, 0);
	else if (size.w > 0)
		calculatedSize.w = size.w;

	if (size.h == SIZE_FILL_PARENT)
		calculatedSize.h = max(sizeAvailable.h, 0);
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

void View::DeleteChildView(int i)
{
	if (i >= 0 && i < GetNumberOfChildren())
	{
		delete children.at(i);
		children.erase(children.begin() + i);
	}
}

void View::DeleteChildView(View* view)
{
	if (view == nullptr)
		return;

	children.erase(std::remove(children.begin(), children.end(), view), children.end());
	delete view;
}

void View::DeleteChildView(std::string id)
{
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i]->GetId() == id)
		{
			DeleteChildView(children[i]);
			return;
		}
	}
}

int View::GetNumberOfChildren()
{
	return children.size();
}

View* View::GetChildView(int i)
{
	return children.at(i);
}

void View::SortChildren(bool(*comparer)(View* v1, View* v2))
{
	sort(children.begin(), children.end(), comparer);
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
	else if (name == "name")
	{
		this->name = value;
		return true;
	}
	else if (name == "tag")
	{
		tags = split(value, '|');
		return true;
	}
	else if (name == "width")
	{
		if (value == "content")
			size.w = SIZE_WRAP_CONTENT;
		else if (value == "*")
			size.w = SIZE_FILL_PARENT;
		else
		{
			stringstream ss(value);
			if ((ss >> size.w).fail() || !(ss >> std::ws).eof())
			{
				throw runtime_error("could not parse width");
			}
		}

		return true;
	}
	else if (name == "height")
	{
		if (value == "content")
			size.h = SIZE_WRAP_CONTENT;
		else if (value == "*")
			size.h = SIZE_FILL_PARENT;
		else
		{
			stringstream ss(value);
			if ((ss >> size.h).fail() || !(ss >> std::ws).eof())
			{
				throw runtime_error("could not parse height");
			}
		}

		return true;
	}
	else if (name == "layoutMargin")
	{
		int top, bottom, left, right;

		stringstream ss(value);

		if ((ss >> top).fail())
			throw runtime_error("could not parse layoutMargin");

		if ((ss >> std::ws).eof())
		{
			// Only one value means we use that value for all sides
			layoutMargin = Box(top, top, top, top);
		}
		else
		{
			if ((ss >> bottom).fail() || (ss >> left).fail() || (ss >> right).fail() || !(ss >> std::ws).eof())
				throw runtime_error("could not parse layoutMargin");
			else
				layoutMargin = Box(top, bottom, left, right);
		}

		return true;
	}
	else if (name == "layoutGravity")
	{
		vector<string> gravityList = split(value, '|');
		for (string g : gravityList)
		{
			if (g == "center_horizontal")
				layoutGravity |= GRAVITY_HCENTER;
			else if (g == "center_vertical")
				layoutGravity |= GRAVITY_VCENTER;
			else if (g == "right")
				layoutGravity |= GRAVITY_RIGHT;
			else if (g == "bottom")
				layoutGravity |= GRAVITY_BOTTOM;
		}

		return true;
	}
	else if (name == "gravity")
	{
		vector<string> gravityList = split(value, '|');
		for (string g : gravityList)
		{
			if (g == "center_horizontal")
				gravity |= GRAVITY_HCENTER;
			else if (g == "center_vertical")
				gravity |= GRAVITY_VCENTER;
			else if (g == "right")
				gravity |= GRAVITY_RIGHT;
			else if (g == "bottom")
				gravity |= GRAVITY_BOTTOM;
		}

		return true;
	}
	else if (name == "action")
	{
		this->action = value;
		return true;
	}
	else if (name == "background")
	{
		this->background = ParseColor(value);
		return true;
	}

	return false;
}

View* View::CopyBase(View* view)
{
	view->SetName(name);
	view->SetSize(size);
	view->SetRelativePosition(relativePosition);
	view->SetLayoutMargin(layoutMargin);
	view->SetLayoutGravity(layoutGravity);
	view->SetGravity(gravity);
	view->SetAction(action);
	view->SetBackground(background);
	view->SetItemTemplate(itemTemplate);

	for (string tag : tags)
	{
		view->AddTag(tag);
	}

	for (View* v : children)
	{
		view->AddChildView(v->Copy());
	}

	return view;
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
	action = FindAndReplace(action, data);
	for (int i = 0; i < tags.size(); i++)
		tags[i] = FindAndReplace(tags[i], data);
}

void View::FillDataAll(std::map<std::string, std::string>& data)
{
	FillData(data);

	for (int i = 0; i < GetNumberOfChildren(); i++)
	{
		View* c = GetChildView(i);
		c->FillDataAll(data);
	}
}

void View::PropagateStateChange(string stateName, string stateValue)
{
	OnStateChange(stateName, stateValue);

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->PropagateStateChange(stateName, stateValue);
	}
}

void View::OnStateChange(string stateName, string stateValue)
{
}

string View::FindAndReplace(string origString, map<string, string>& values)
{
	int searchFrom = 0;
	string str = origString;

	while (true)
	{
		size_t foundStart = str.find_first_of('{', searchFrom);
		if (foundStart == string::npos)
			break;

		size_t foundEnd = str.find_first_of('}', foundStart + 1);
		if (foundEnd == string::npos)
			break;

		try
		{
			string key = str.substr(foundStart + 1, foundEnd - foundStart - 1);
			string replaceWith = ThemeManager::ProcessPath(values.at(key));
			int keyLength = foundEnd - foundStart - 1 + 2; // + 2 including brackets
			int replaceWithLength = replaceWith.length();
			int sizeChange = replaceWithLength - keyLength;

			searchFrom = foundEnd + sizeChange + 1;
			str.replace(foundStart, keyLength, replaceWith);
		}
		catch (exception& ex)
		{
			searchFrom = foundEnd + 1;
		}
	}

	return str;
}
