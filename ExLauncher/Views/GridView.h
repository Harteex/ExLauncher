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
