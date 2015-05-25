#ifndef _I_SELECTION_VIEW_H_
#define _I_SELECTION_VIEW_H_

/*********************************************/

#include "View.h"
#include "../structures.h"
#include "../ScreenSystem/InputState.h"
#include <vector>

/*********************************************/

class ISelectionHandler
{
public:
	virtual ~ISelectionHandler() {}

	virtual void HandleInput(InputState* input) = 0;
	virtual int GetSelectedIndex() = 0;
	virtual View* GetSelectedView() = 0;
	virtual bool SelectByName(std::string name) = 0;
	virtual bool SelectByIndex(int index) = 0;
	virtual View* SelectNext(Direction direction) = 0;
};

/*********************************************/

#endif
