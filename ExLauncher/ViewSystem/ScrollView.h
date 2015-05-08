#ifndef _SCROLL_VIEW_H_
#define _SCROLL_VIEW_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../ViewSystem/CollectionView.h"
#include "../structures.h"
#include "../ScreenSystem/InputState.h"
#include <vector>

/*********************************************/

class ScrollView : public CollectionView
{
private:
	int scrollOffset;
	int scrollToTargetOffset;
	Orientation orientation;

	void DrawChildren(Position offset, Rectangle viewBounds);
protected:
	void SetOrientation(Orientation orientation);
public:
	ScrollView();
	virtual void OnUpdate();

	void ScrollTo(int offset);
	void ScrollTo(View* child);
};

/*********************************************/

#endif
