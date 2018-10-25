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

#ifndef _I_SELECTION_VIEW_H_
#define _I_SELECTION_VIEW_H_

/*********************************************/

#include "View.h"
#include "../structures.h"
#include "../ScreenSystem/InputState.h"
#include <vector>

/*********************************************/

class ISelectionHandler
{
public:
	virtual ~ISelectionHandler() {}

	virtual void HandleInput(InputState* input) = 0;
	virtual int GetSelectedIndex() = 0;
	virtual View* GetSelectedView() = 0;
	virtual bool SelectByName(std::string name) = 0;
	virtual bool SelectByIndex(int index) = 0;
	virtual bool SelectById(std::string id) = 0;
	virtual View* SelectNext(Direction direction) = 0;
};

/*********************************************/

#endif
