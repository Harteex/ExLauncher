#ifndef _TAB_PANEL_H_
#define _TAB_PANEL_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include <vector>

/*********************************************/

class TabPanel : public View
{
public:
	TabPanel();
	virtual ~TabPanel();
	bool Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer);
	void OnLayoutChange();
	virtual View* Copy();

	virtual bool SetProperty(std::string name, std::string value);
};

/*********************************************/

#endif
