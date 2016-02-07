/*
Copyright 2016 Andreas Bjerkeholt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
