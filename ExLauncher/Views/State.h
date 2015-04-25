#ifndef _STATE_H_
#define _STATE_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include "FramePanel.h"

/*********************************************/

class State : public FramePanel
{
private:
	std::string stateName;
	std::string stateValue;
public:
	State();
	~State();
	View* Copy();
	void AddChildView(View* view);

	void SetStateName(std::string name);
	void SetStateValue(std::string value);
	bool SetProperty(std::string name, std::string value);
	void OnStateChange(std::string stateName, std::string stateValue);
};

/*********************************************/

#endif
