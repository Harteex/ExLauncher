#include "TabPanel.h"
#include <algorithm>

using namespace std;

TabPanel::TabPanel()
{
	contentSize.w = 0;
	contentSize.h = 0;
}

TabPanel::~TabPanel()
{
}

bool TabPanel::Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer)
{
	this->renderer = renderer;
	isInitialized = true;
	return true;
}

void TabPanel::OnLayoutChange()
{
	
}

View* TabPanel::Copy()
{
	TabPanel* view = new TabPanel();

	CopyBase(view);

	return view;
}

bool TabPanel::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	return false;
}
