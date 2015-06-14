#ifndef _TAB_PANEL_H_
#define _TAB_PANEL_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../ViewSystem/ISelectionHandler.h"
#include "../structures.h"
#include "Label.h"
#include "TabStrip.h"
#include <vector>

/*********************************************/

class TabPanel : public View, public ISelectionHandler
{
private:
	int selectedIndex;
	std::vector<ISelectionHandler*> childrenSelectionHandlers;
	Size tabAreaSize;
	TabStrip* tabStrip;
	
	void DoSelectTabByIndex(int index);
public:
	TabPanel();
	virtual ~TabPanel();
	bool Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer);
	void OnUpdate();
	void OnDraw(SDL_Renderer* renderer, Position offset);
	void OnLayoutChange();
	virtual View* Copy();

	virtual bool SetProperty(std::string name, std::string value);
	void AddChildView(View* view);

	void HandleInput(InputState* input);
	int GetSelectedIndex();
	View* GetSelectedView();
	bool SelectByName(std::string name);
	bool SelectByIndex(int index);
	View* SelectNext(Direction direction);
};

/*********************************************/

#endif
