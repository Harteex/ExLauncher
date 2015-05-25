#ifndef _COLLECTION_VIEW_H_
#define _COLLECTION_VIEW_H_

/*********************************************/

#include "View.h"
#include "ISelectionHandler.h"
#include "../structures.h"
#include "../ScreenSystem/InputState.h"
#include <vector>

/*********************************************/

class CollectionView : public View, public ISelectionHandler
{
protected:
	int selectedIndex;
	View* selectedItem;
public:
	CollectionView();

	void HandleInput(InputState* input);
	int GetSelectedIndex();
	View* GetSelectedView();
	bool SelectByName(std::string name);
	bool SelectByIndex(int index);
	virtual void OnSelectionChanged() = 0;
};

/*********************************************/

#endif
