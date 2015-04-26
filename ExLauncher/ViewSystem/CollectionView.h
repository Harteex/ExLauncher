#ifndef _COLLECTION_VIEW_H_
#define _COLLECTION_VIEW_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include "../ScreenSystem/InputState.h"
#include <vector>

/*********************************************/

class CollectionView : public View
{
protected:
	Position selectedPosition;
	View* selectedItem;
public:
	CollectionView();

	void HandleInput(InputState* input);
	Position GetSelectedPosition();
	virtual View* GetSelectedItem() = 0;
	virtual View* SelectNext(Direction direction) = 0;
	virtual View* Select(Position position) = 0;
};

/*********************************************/

#endif
