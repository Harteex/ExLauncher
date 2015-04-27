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
	Selector* view = new Selector();

	CopyBase(view);

	return view;
}

bool Selector::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	return false;
}
