#include "TabStrip.h"
#include <algorithm>

using namespace std;

TabStrip::TabStrip()
{
	contentSize.w = 0;
	contentSize.h = 0;
	selectedTab = 0;
	tabSize = Size(SIZE_WRAP_CONTENT, SIZE_WRAP_CONTENT);
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
		label->CalculateLayout(Position(0, 0), Size(-1, -1));
	}

	isInitialized = true;
	return true;
}

void TabStrip::OnDraw(SDL_Renderer* renderer, Position offset)
{
	// TODO offset etc?

	if (selectedTab >= tabs.size() || selectedTab < 0)
		return;

	Size selectedTabSize = tabs[selectedTab]->GetCalculatedSize();

	int xPos = 0;
	if (pinOrigin == PinOriginCenter)
		xPos = calculatedSize.w / 2 - selectedTabSize.w / 2;
	else if (pinOrigin == PinOriginRight)
		xPos = calculatedSize.w - selectedTabSize.w;

	Position selectedPos = Position(xPos, 0) + pinPosition;

	switch (tabMode)
	{
	case TabModeNormal:
		{
			tabs[selectedTab]->SetTextColor(Color(0xff, 0xff, 0xff, 0xff));
			tabs[selectedTab]->Draw(selectedPos);

			int x = selectedPos.x;
			int i = selectedTab;
			while (x >= 0)
			{
				i--;
				if (i < 0)
					i = tabs.size() - 1;

				x -= tabs[i]->GetCalculatedSize().w + tabs[i]->GetLayoutMargin().left;

				tabs[i]->SetTextColor(Color(0xb0, 0xb0, 0xb0, 0xff));
				tabs[i]->Draw(Position(x, selectedPos.y));
			}

			x = selectedPos.x;
			i = selectedTab;
			while (true)
			{
				x += tabs[i]->GetCalculatedSize().w + tabs[i]->GetLayoutMargin().right;

				if (x >= calculatedSize.w)
					break;

				i = (i + 1) % tabs.size();

				tabs[i]->SetTextColor(Color(0xb0, 0xb0, 0xb0, 0xff));
				tabs[i]->Draw(Position(x, selectedPos.y));
			}
		}
		break;
	case TabModeShowSingle:
		tabs[selectedTab]->Draw(selectedPos);
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
	view->SetTabSize(tabSize);

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

Size TabStrip::GetTabSize()
{
	return tabSize;
}

void TabStrip::SetTabSize(Size tabSize)
{
	this->tabSize = tabSize;
}

// TODO Currently not usable via XML, so haven't bothered implementing this method properly
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
	label->SetLayoutMargin(Box(0, 0, 10, 10));
	label->SetGravity(GRAVITY_VCENTER | GRAVITY_HCENTER);
	label->SetText(name);
	label->SetParentView(this);
	label->SetRelativePosition(0, 0);
	label->SetTextSize(18);
	if (isInitialized)
	{
		label->Initialize(resourceManager, renderer);
		label->CalculateLayout(Position(0, 0), Size(-1, -1));
	}

	tabs.push_back(label);
}

void TabStrip::SelectTab(int index)
{
	// FIXME bounds check
	selectedTab = index;
}
