#include "Selector.h"
#include <algorithm>

using namespace std;

Selector::Selector()
{
	stateName = "";
	stateValue = "";
}

Selector::~Selector()
{
}

View* Selector::Copy()
{
	Selector* panel = new Selector();

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

void Selector::SetStateName(std::string name)
{
	this->stateName = name;
}

void Selector::SetStateValue(std::string value)
{
	this->stateValue = value;
}

bool Selector::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	if (name == "stateSelected")
	{
		stateName = "stateSelected";
		stateValue = value;
		return true;
	}

	return false;
}

void Selector::OnStateChange(string stateName, string stateValue)
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
