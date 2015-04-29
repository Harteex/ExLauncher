#ifndef _GRID_VIEW_H_
#define _GRID_VIEW_H_

/*********************************************/

#include "../ViewSystem/CollectionView.h"
#include "../structures.h"
#include <vector>

/*********************************************/

class GridView : public CollectionView
{
private:
	//int selectedPosition;
	int itemCountSecondaryDirection;
	Orientation orientation;

	int itemMargin;

	/*Position position;
	Size tileViewSize;
	Size tileSize;
	int tileMargin;
	int scrollDistance;
	double zoom;*/

	//void Scroll();
	//void SetElementPosition(FormElement* element, Position gridPosition);
	//Position GetPxPositionForPosition(Position position);
	void GetRowsAndColumns(int& outRows, int& outColumns);
	Position GetPositionForItemIndex(int elementNo);
	int GetItemIndexForPosition(Position position);
public:
	GridView();
	~GridView();
	bool Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer);
	void Update();
	//void SetTransition(TransitionEffect effect, double value);
	//void ClearTransitions();
	void OnLayoutChange();
	View* Copy();

	int GetItemCountSecondaryDirection();
	void SetItemCountSecondaryDirection(int count);
	Orientation GetOrientation();
	void SetOrientation(Orientation orientation);
	bool SetProperty(std::string name, std::string value);

	View* GetSelectedItem();
	View* SelectNext(Direction direction);
	View* Select(Position position);
};

/*********************************************/

#endif
