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

#ifndef _LIST_VIEW_H_
#define _LIST_VIEW_H_

/*********************************************/

#include "../ViewSystem/ScrollView.h"
#include "../structures.h"
#include <vector>

/*********************************************/

class ListView : public ScrollView
{
private:
	Orientation orientation;
	int itemSize;
	int leadingContentInset;
	int trailingContentInset;

	Position GetPositionForItemIndex(int elementNo);
	int GetItemIndexForPosition(Position position);
public:
	ListView();
	~ListView();
	void OnInitialize();
	void OnLayoutChange();
	View* Copy();
	void AddChildView(View* view);

	Orientation GetOrientation();
	void SetOrientation(Orientation orientation);
	int GetItemSize();
	void SetItemSize(int itemSize);
	void SetContentInset(int leading, int trailing);
	bool SetProperty(std::string name, std::string value);

	View* GetSelectedItem();
	View* SelectNext(Direction direction);
	View* Select(Position position);
	void OnSelectionChanged();
};

/*********************************************/

#endif
