#include "TabStrip.h"
#include <algorithm>
#include "../ScreenSystem/Screen.h"

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
	isAnimating = false;
	movingTabAnimator = new Animator(0.2, TransitionDirectionOn);
}

TabStrip::~TabStrip()
{
	delete movingTabAnimator;
}

bool TabStrip::OnInitialize()
{
	for (Label* label : tabs)
	{
		if (!label->Initialize(context))
			return false;

		label->CalculateLayout(Size(-1, -1));
	}

	return true;
}

void TabStrip::OnUpdate()
{
	if (isAnimating)
	{
		movingTabAnimator->Update();

		isAnimating = !movingTabAnimator->IsDone();
	}
}

Position TabStrip::GetSelectedTabPosition(View* selectedTab)
{
	Size selectedTabSize = selectedTab->GetCalculatedSize();

	int xPos = 0;
	if (pinOrigin == PinOriginCenter)
		xPos = calculatedSize.w / 2 - selectedTabSize.w / 2;
	else if (pinOrigin == PinOriginRight)
		xPos = calculatedSize.w - selectedTabSize.w;

	Position selectedPos = Position(xPos, 0) + pinPosition;

	return selectedPos;
}

void TabStrip::OnDraw(SDL_Renderer* renderer, Position offset)
{
	// TODO offset argument etc?

	if (tabs.size() == 0)
		return;

	if (selectedTab >= tabs.size() || selectedTab < 0)
		return;

	Position selectedPos;

	if (isAnimating)
		selectedPos = GetAnimatedPosition(movingTabAnimator->GetProgress());
	else
		selectedPos = GetSelectedTabPosition(tabs[selectedTab]);
	

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
	if (IsInitialized())
	{
		label->Initialize(context);
		label->CalculateLayout(Size(-1, -1));
	}

	tabs.push_back(label);
}

void TabStrip::SelectTab(int index)
{
	// FIXME bounds check
	selectedTab = index;
}

void TabStrip::SwitchTab(Direction direction)
{
	if (direction != DirectionLeft && direction != DirectionRight)
		return;

	if (tabs.size() == 0)
		return;

	if (selectedTab >= tabs.size() || selectedTab < 0)
		return;

	Position selectedPos = GetSelectedTabPosition(tabs[selectedTab]);

	int x = selectedPos.x;
	int i = selectedTab;
	if (direction == DirectionLeft)
	{
		i--;
		if (i < 0)
			i = tabs.size() - 1;

		x -= tabs[i]->GetCalculatedSize().w + tabs[i]->GetLayoutMargin().left;
	}
	else
	{
		x += tabs[i]->GetCalculatedSize().w + tabs[i]->GetLayoutMargin().right;

		i = (i + 1) % tabs.size();
	}

	animationPositionFrom = Position(x, selectedPos.y);
	animationPositionTo = GetSelectedTabPosition(tabs[i]);
	movingTabAnimator->Reset();
	isAnimating = true;

	SelectTab(i);
}

Position TabStrip::GetAnimatedPosition(double animationProgress)
{
	double dX = animationPositionFrom.x - animationPositionTo.x;
	double dY = animationPositionFrom.y - animationPositionTo.y;

	Position pos;
	pos.x = animationPositionFrom.x - (dX * animationProgress);
	pos.y = animationPositionFrom.y - (dY * animationProgress);

	return pos;
}
