#ifndef _SELECTOR_H_
#define _SELECTOR_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include "FramePanel.h"

/*********************************************/

class Selector : public FramePanel
{
private:
	std::string stateName;
	std::string stateValue;
public:
	Selector();
	~Selector();
	View* Copy();

	void SetStateName(std::string name);
	void SetStateValue(std::string value);
	bool SetProperty(std::string name, std::string value);
	void OnStateChange(std::string stateName, std::string stateValue);
};

/*********************************************/

#endif
