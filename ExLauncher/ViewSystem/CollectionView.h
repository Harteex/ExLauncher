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

#ifndef _COLLECTION_VIEW_H_
#define _COLLECTION_VIEW_H_

/*********************************************/

#include "View.h"
#include "ISelectionHandler.h"
#include "../structures.h"
#include "../ScreenSystem/InputState.h"
#include <vector>

/*********************************************/

class CollectionView : public View, public ISelectionHandler
{
protected:
	int selectedIndex;
public:
	CollectionView();

	void HandleInput(InputState* input);
	int GetSelectedIndex();
	View* GetSelectedView();
	bool SelectByName(std::string name);
	bool SelectByIndex(int index);
	bool SelectById(std::string id);
	virtual void OnSelectionChanged() = 0;
};

/*********************************************/

#endif
