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

	Position GetPositionForItemIndex(int elementNo);
	int GetItemIndexForPosition(Position position);
public:
	ListView();
	~ListView();
	bool OnInitialize();
	void OnLayoutChange();
	View* Copy();
	void AddChildView(View* view);

	Orientation GetOrientation();
	void SetOrientation(Orientation orientation);
	int GetItemSize();
	void SetItemSize(int itemSize);
	bool SetProperty(std::string name, std::string value);

	View* GetSelectedItem();
	View* SelectNext(Direction direction);
	View* Select(Position position);
	void OnSelectionChanged();
};

/*********************************************/

#endif
