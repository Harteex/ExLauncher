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
protected:
	int scrollOffset;
	int scrollToTargetOffset;

	void DrawChildren(Position offset, Rectangle viewBounds);
public:
	ScrollView();
	virtual void OnUpdate();

	void ScrollTo(int offset);
	void ScrollTo(View* child);
};

/*********************************************/

#endif
