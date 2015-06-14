#ifndef _TAB_STRIP_H_
#define _TAB_STRIP_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../ViewSystem/ISelectionHandler.h"
#include "../structures.h"
#include "Label.h"
#include <vector>
#include "../Animator.h"

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
	Animator* movingTabAnimator;
	Position animationPositionFrom;
	Position animationPositionTo;
	bool isAnimating;

	Position GetSelectedTabPosition(View* selectedTab);
	Position GetAnimatedPosition(double animationProgress);
public:
	TabStrip();
	virtual ~TabStrip();
	bool Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer);
	void OnUpdate();
	void OnDraw(SDL_Renderer* renderer, Position offset);
	void OnLayoutChange();
	virtual View* Copy();

	PinOrigin GetPinOrigin();
	void SetPinOrigin(PinOrigin pinOrigin);
	Position GetPinPosition();
	void SetPinPosition(Position pinPosition);
	TabMode GetTabMode();
	void SetTabMode(TabMode tabMode);
	Size GetTabSize();
	void SetTabSize(Size tabSize);
	virtual bool SetProperty(std::string name, std::string value);

	void AddTab(std::string name);
	void RemoveTab(std::string name);
	void RemoveTab(int index);
	void SelectTab(int index);
	void SwitchTab(Direction direction);
};

/*********************************************/

#endif
