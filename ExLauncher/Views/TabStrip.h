#ifndef _TAB_STRIP_H_
#define _TAB_STRIP_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../ViewSystem/ISelectionHandler.h"
#include "../structures.h"
#include "Label.h"
#include <vector>

/*********************************************/

enum PinOrigin {PinOriginLeft, PinOriginCenter, PinOriginRight};
enum TabMode {TabModeNormal, TabModeShowSingle};

/*********************************************/

class TabStrip : public View
{
private:
	int selectedTab;
	std::vector<Label*> tabs;
	Size tabSize;
	PinOrigin pinOrigin;
	Position pinPosition;
	ResourceManager* resourceManager;
	TabMode tabMode;
public:
	TabStrip();
	virtual ~TabStrip();
	bool Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer);
	void OnDraw(SDL_Renderer* renderer, Position offset);
	void OnLayoutChange();
	virtual View* Copy();

	PinOrigin GetPinOrigin();
	void SetPinOrigin(PinOrigin pinOrigin);
	Position GetPinPosition();
	void SetPinPosition(Position pinPosition);
	TabMode GetTabMode();
	void SetTabMode(TabMode tabMode);
	virtual bool SetProperty(std::string name, std::string value);

	void AddTab(std::string name);
	void RemoveTab(std::string name);
	void RemoveTab(int index);
	void SelectTab(int index);
};

/*********************************************/

#endif
