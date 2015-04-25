#include "State.h"
#include <algorithm>

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
	State* panel = new State();

	panel->SetSize(size);
	panel->SetRelativePosition(relativePosition);
	panel->SetStateName(stateName);
	panel->SetStateValue(stateValue);

	for (View* view : children)
	{
		panel->AddChildView(view->Copy());
	}

	return panel;
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

	if (name == "stateSelected")
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
