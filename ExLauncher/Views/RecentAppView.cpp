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

#include "RecentAppView.h"
#include <sstream>
#include <algorithm>
#include "../ScreenSystem/Screen.h"
#include "../ScreenSystem/ScreenManager.h"

using namespace std;

RecentAppView::RecentAppView()
{
	recentIndex = 0;
	category = "";
	origView = NULL;
}

RecentAppView::~RecentAppView()
{
	if (origView != NULL)
		delete origView;
}

void RecentAppView::OnInitialize()
{
	tags.push_back("recent");

	if (GetNumberOfChildren() > 0)
		origView = GetChildView(0)->Copy();

	FillViewWithRecent();
}

View* RecentAppView::Copy()
{
	RecentAppView* view = new RecentAppView();

	CopyBase(view);
	view->SetRecentIndex(recentIndex);
	view->SetCategory(category);

	return view;
}

void RecentAppView::AddChildView(View* view)
{
	if (GetNumberOfChildren() > 0)
		throw runtime_error("RecentAppView can only have one direct child");

	View::AddChildView(view);
}

void RecentAppView::SetRecentIndex(int recentIndex)
{
	this->recentIndex = recentIndex;
}

void RecentAppView::SetCategory(std::string category)
{
	this->category = category;
}

bool RecentAppView::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	if (name == "recentIndex")
	{
		stringstream ss(value);
		if ((ss >> recentIndex).fail() || !(ss >> std::ws).eof())
		{
			throw runtime_error("could not parse recentIndex");
		}

		return true;
	}
	else if (name == "category")
	{
		category = value;
		return true;
	}

	return false;
}

void RecentAppView::FillViewWithRecent()
{
	View* v = GetChildView(0);
	if (v != nullptr)
	{
		App* app = context->GetAppManager()->GetRecent(recentIndex);
		if (app != nullptr)
		{
			SetId(app->GetData("id", ""));
			SetAction("app");
			SetActionArgs(app->GetExec());

			v->FillDataAll(app->GetAllData());

			PropagateStateChange("stateRecentType", "recentItem"); // TODO could be pinnedItem too in the future
		}
		else
		{
			map<string, string> tempMap;
			tempMap["iconId"] = "appIconDefault";
			v->FillDataAll(tempMap);

			PropagateStateChange("stateRecentType", "noItem");
		}
	}
}

void RecentAppView::OnRecentChanged()
{
	while (GetNumberOfChildren() > 0)
		DeleteChildView(0);

	if (origView != nullptr)
		AddChildView(origView->Copy());

	FillViewWithRecent();
}
