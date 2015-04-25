#include "Selector.h"
#include <algorithm>

using namespace std;

Selector::Selector()
{
}

Selector::~Selector()
{
}

View* Selector::Copy()
{
	Selector* panel = new Selector();

	panel->SetSize(size);
	panel->SetRelativePosition(relativePosition);

	for (View* view : children)
	{
		panel->AddChildView(view->Copy());
	}

	return panel;
}

bool Selector::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	return false;
}
