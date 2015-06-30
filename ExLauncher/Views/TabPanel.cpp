#include "TabPanel.h"
#include <algorithm>

using namespace std;

TabPanel::TabPanel()
{
	contentSize.w = 0;
	contentSize.h = 0;
	selectedIndex = 0;
	tabAreaSize = Size(SIZE_MATCH_PARENT, 30);

	tabStrip = new TabStrip();
	tabStrip->SetSize(tabAreaSize);
	tabStrip->SetRelativePosition(0, 0);
	tabStrip->SetParentView(this);
	//tabStrip->SetTabSize(Size(100, 30));
}

TabPanel::~TabPanel()
{
	delete tabStrip;
}

bool TabPanel::OnInitialize()
{
	return tabStrip->Initialize(context);
}

void TabPanel::OnUpdate()
{
	tabStrip->Update();
}

void TabPanel::OnDraw(SDL_Renderer* renderer, Position offset)
{
	// TODO offset etc
	tabStrip->Draw();
}

void TabPanel::OnLayoutChange()
{
	contentSize = calculatedSize; // This will cause (0, 0) on wrap_content usage, but it's ok since we don't support wrap_content here

	tabStrip->CalculateLayout(Position(0, 0), calculatedSize);

	for (int i = 0; i < children.size(); i++)
	{
		View* v = children.at(i);

		Box childMargin = v->GetLayoutMargin();
		v->SetRelativePosition(Position(childMargin.left, childMargin.top) + Position(0, tabAreaSize.h));
		Size sizeAreaForChild;
		sizeAreaForChild.w = max(calculatedSize.w - (childMargin.left + childMargin.right), 0);
		sizeAreaForChild.h = max(calculatedSize.h - (childMargin.top + childMargin.bottom), 0);
		v->CalculateLayout(Position(0, 0), sizeAreaForChild);
	}
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

void TabPanel::AddChildView(View* view)
{
	if (children.size() > 0)
		view->SetVisible(false);

	View::AddChildView(view);
	childrenSelectionHandlers.push_back(dynamic_cast<ISelectionHandler*>(view));
	tabStrip->AddTab(view->GetName());
}

void TabPanel::HandleInput(InputState* input)
{
	if (input->GameKeyRepeat(GAMEKEY_TRIGGER_L))
		SelectNext(DirectionLeft);
	else if (input->GameKeyRepeat(GAMEKEY_TRIGGER_R))
		SelectNext(DirectionRight);
	else
	{
		if (children.size() > 0)
		{
			// Let the tab handle input if it's able to
			ISelectionHandler* activeTab = childrenSelectionHandlers[selectedIndex];
			if (activeTab != NULL)
				activeTab->HandleInput(input);
		}
	}
}

int TabPanel::GetSelectedIndex()
{
	return selectedIndex;
}

View* TabPanel::GetSelectedView()
{
	if (selectedIndex < 0 || selectedIndex >= children.size())
		return NULL;

	return children[selectedIndex];
}

bool TabPanel::SelectByName(std::string name)
{
	for (int i = 0; i < children.size(); i++)
	{
		View* v = children[i];

		if (v->GetName() == name)
			return SelectByIndex(i);
	}

	// No view with the specified name was found
	return false;
}

void TabPanel::DoSelectTabByIndex(int index)
{
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->SetVisible(i == index);
	}

	selectedIndex = index;
}

bool TabPanel::SelectByIndex(int index)
{
	if (index < 0 || index >= children.size())
		return false;

	DoSelectTabByIndex(index);
	
	tabStrip->SelectTab(index);
	return true;
}

View* TabPanel::SelectNext(Direction direction)
{
	if (children.size() == 0)
		return NULL;

	int newIndex;
	if (direction == DirectionLeft)
	{
		newIndex = selectedIndex - 1;
		if (newIndex < 0)
			newIndex = children.size() - 1;

		DoSelectTabByIndex(newIndex);
		tabStrip->SwitchTab(DirectionLeft);
	}
	else if (direction == DirectionRight)
	{
		newIndex = selectedIndex + 1;
		if (newIndex >= children.size())
			newIndex = 0;

		DoSelectTabByIndex(newIndex);
		tabStrip->SwitchTab(DirectionRight);
	}

	return NULL;
}
