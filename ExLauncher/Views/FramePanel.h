#ifndef _FRAME_PANEL_H_
#define _FRAME_PANEL_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include <vector>

/*********************************************/

class FramePanel : public View
{
public:
	FramePanel();
	~FramePanel();
	bool Initialize(ResourceManager* resourceManager, SDL_Renderer* renderer);
	void Update();
	void Draw(SDL_Renderer* renderer);
	void OnLayoutChange();
	virtual View* Copy();

	virtual bool SetProperty(std::string name, std::string value);
};

/*********************************************/

#endif
