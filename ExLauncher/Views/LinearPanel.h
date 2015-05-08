#ifndef _LINEAR_PANEL_H_
#define _LINEAR_PANEL_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include <vector>

/*********************************************/

class LinearPanel : public View
{
private:
	Orientation orientation;
public:
	LinearPanel();
	~LinearPanel();
	bool Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer);
	void OnLayoutChange();
	View* Copy();

	Orientation GetOrientation();
	void SetOrientation(Orientation orientation);
	bool SetProperty(std::string name, std::string value);
};

/*********************************************/

#endif
