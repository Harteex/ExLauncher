#include "TabStrip.h"
#include <algorithm>

using namespace std;

TabStrip::TabStrip()
{
	contentSize.w = 0;
	contentSize.h = 0;
	selectedTab = 0;
	tabSize = Size(120, 30);
	pinOrigin = PinOriginCenter;
	pinPosition = Position(0, 0);
	tabMode = TabModeNormal;
}

TabStrip::~TabStrip()
{
}

bool TabStrip::Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer)
{
	this->renderer = renderer;
	this->resourceManager = resourceManager;

	for (Label* label : tabs)
	{
		label->Initialize(resourceManager, renderer);
		label->CalculateLayout(Position(0, 0), tabSize);
	}

	isInitialized = true;
	return true;
}

void TabStrip::OnDraw(SDL_Renderer* renderer, Position offset)
{
	if (selectedTab >= tabs.size() || selectedTab < 0)
		return;

	int xPos = 0;
	if (pinOrigin == PinOriginCenter)
		xPos = calculatedSize.w / 2 - tabSize.w / 2;
	else if (pinOrigin == PinOriginRight)
		xPos = calculatedSize.w - tabSize.w;

	Position pos = Position(xPos, 0) + pinPosition;

	switch (tabMode)
	{
	case TabModeNormal:
		{
			tabs[selectedTab]->SetTextColor(Color(0xff, 0xff, 0xff, 0xff));
			tabs[selectedTab]->Draw(-pos);

			int x = pos.x - tabSize.w;
			int i = selectedTab;
			while (x > -tabSize.w)
			{
				i--;
				if (i < 0)
					i = tabs.size() - 1;

				tabs[i]->SetTextColor(Color(0xb0, 0xb0, 0xb0, 0xff));
				tabs[i]->Draw(-Position(x, pos.y));

				x -= tabSize.w;
			}

			x = pos.x + tabSize.w;
			i = selectedTab;
			while (x < calculatedSize.w)
			{
				i = (i + 1) % tabs.size();

				tabs[i]->SetTextColor(Color(0xb0, 0xb0, 0xb0, 0xff));
				tabs[i]->Draw(-Position(x, pos.y));

				x += tabSize.w;
			}
		}
		break;
	case TabModeShowSingle:
		tabs[selectedTab]->Draw(-pos);
		break;
	default:
		break;
	}
}

void TabStrip::OnLayoutChange()
{
	//
}

View* TabStrip::Copy()
{
	TabStrip* view = new TabStrip();

	view->SetPinOrigin(pinOrigin);
	view->SetPinPosition(pinPosition);
	view->SetTabMode(tabMode);

	for (Label* v : tabs)
	{
		view->AddTab(v->GetText());
	}

	CopyBase(view);

	return view;
}

PinOrigin TabStrip::GetPinOrigin()
{
	return pinOrigin;
}

void TabStrip::SetPinOrigin(PinOrigin pinOrigin)
{
	this->pinOrigin = pinOrigin;
}

Position TabStrip::GetPinPosition()
{
	return pinPosition;
}

void TabStrip::SetPinPosition(Position pinPosition)
{
	this->pinPosition = pinPosition;
}

TabMode TabStrip::GetTabMode()
{
	return tabMode;
}

void TabStrip::SetTabMode(TabMode tabMode)
{
	this->tabMode = tabMode;
}

// TODO Currently not usable via XML, so haven't bothered implementing this function properly
bool TabStrip::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	return false;
}

void TabStrip::AddTab(string name)
{
	Label* label = new Label();
	label->SetSize(tabSize);
	label->SetGravity(GRAVITY_VCENTER | GRAVITY_HCENTER);
	label->SetText(name);
	label->SetParentView(this);
	label->SetRelativePosition(0, 0);
	label->SetTextSize(18);
	if (isInitialized)
	{
		label->Initialize(resourceManager, renderer);
		label->CalculateLayout(Position(0, 0), tabSize);
	}

	tabs.push_back(label);
}

void TabStrip::SelectTab(int index)
{
	// FIXME bounds check
	selectedTab = index;
}
