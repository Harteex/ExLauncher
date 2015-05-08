#ifndef _LINEAR_PANEL_H_
#define _LINEAR_PANEL_H_

/*********************************************/

#include "../ViewSystem/ScrollView.h"
#include "../structures.h"
#include <vector>

/*********************************************/

class LinearPanel : public ScrollView
{
private:
	Orientation orientation;

	Position GetPositionForItemIndex(int elementNo);
	int GetItemIndexForPosition(Position position);
public:
	LinearPanel();
	~LinearPanel();
	bool Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer);
	void OnLayoutChange();
	View* Copy();

	Orientation GetOrientation();
	void SetOrientation(Orientation orientation);
	bool SetProperty(std::string name, std::string value);

	View* GetSelectedItem();
	View* SelectNext(Direction direction);
	View* Select(Position position);
};

/*********************************************/

#endif
