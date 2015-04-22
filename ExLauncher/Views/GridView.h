#ifndef _GRID_VIEW_H_
#define _GRID_VIEW_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include <vector>

/*********************************************/

class GridView : public View
{
private:
	int selectedPosition;
	int itemCountSecondaryDirection;
	Orientation orientation;

	int itemMargin;

	/*Position position;
	Size tileViewSize;
	Size tileSize;
	int tileMargin;
	int layoutMargin;
	//std::vector<FormElement*> formElements;
	Position selectedTile;
	Orientation orientation;
	int iconCountSecondaryDirection;
	int scrollDistance;
	double zoom;*/

	/*void Scroll();
	//void SetElementPosition(FormElement* element, Position gridPosition);
	Position GetPxPositionForPosition(Position position);
	void GetRowsAndColumns(int* outRows, int* outColumns);
	Position GetPositionForElementIndex(int elementNo);
	int GetElementIndexForPosition(Position position);*/
public:
	GridView();
	~GridView();
	/*void AddElement(FormElement* formElement);
	void AddElements(std::list<FormElement*> formElements);*/
	bool Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer);
	void Update();
	void Draw(SDL_Renderer* renderer);
	//FormElement* GetElement(int position);
	//int GetSelectedPosition();
	//FormElement* SelectNextElement(Direction direction);
	//void SetTransition(TransitionEffect effect, double value);
	//void ClearTransitions();
	void OnLayoutChange();
	View* Copy();

	int GetItemCountSecondaryDirection();
	void SetItemCountSecondaryDirection(int count);
	Orientation GetOrientation();
	void SetOrientation(Orientation orientation);
	bool SetProperty(std::string name, std::string value);
};

/*********************************************/

#endif
