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

#ifndef _GRID_VIEW_H_
#define _GRID_VIEW_H_

/*********************************************/

#include "../ViewSystem/ScrollView.h"
#include "../structures.h"
#include <vector>

/*********************************************/

class GridView : public ScrollView
{
private:
	int itemCountSecondaryDirection;

	Orientation orientation;
	Size gridSpacing;
	Size itemSize;

	void GetRowsAndColumns(int& outRows, int& outColumns);
	Position GetPositionForItemIndex(int elementNo);
	int GetItemIndexForPosition(Position position);
public:
	GridView();
	~GridView();
	bool OnInitialize();
	//void SetTransition(TransitionEffect effect, double value);
	//void ClearTransitions();
	void OnLayoutChange();
	View* Copy();

	Orientation GetOrientation();
	void SetOrientation(Orientation orientation);
	Size GetGridSpacing();
	void SetGridSpacing(Size gridSpacing);
	Size GetItemSize();
	void SetItemSize(Size itemSize);
	bool SetProperty(std::string name, std::string value);

	View* GetSelectedItem();
	View* SelectNext(Direction direction);
	bool Select(Position position);
	void OnSelectionChanged();
};

/*********************************************/

#endif
