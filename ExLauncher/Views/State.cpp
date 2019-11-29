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

#include "State.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

State::State()
{
	stateName = "";
	stateValue = "";
}

State::~State()
{
}

View* State::Copy()
{
	State* view = new State();

	CopyBase(view);
	view->SetStateName(stateName);
	view->SetStateValue(stateValue);

	return view;
}

void State::AddChildView(View* view)
{
	if (children.size() > 0)
		throw runtime_error("SelectorState can only have one direct child");

	SetSize(view->GetSize());

	View::AddChildView(view);
}

void State::SetStateName(std::string name)
{
	this->stateName = name;
}

void State::SetStateValue(std::string value)
{
	this->stateValue = value;
}

bool State::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	if (name == "stateSelected" || name == "stateRecentType")
	{
		stateName = name;
		stateValue = value;
		return true;
	}

	return false;
}

void State::OnStateChange(string stateName, string stateValue)
{
	if (this->stateName == stateName)
	{
		bool showChildren = this->stateValue == "" || this->stateValue == stateValue;
		for (View* view : children)
		{
			view->SetVisible(showChildren);
		}
	}
}
