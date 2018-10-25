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

#ifndef _TAB_PANEL_H_
#define _TAB_PANEL_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../ViewSystem/ISelectionHandler.h"
#include "../structures.h"
#include "Label.h"
#include "TabStrip.h"
#include <vector>

/*********************************************/

class TabPanel : public View, public ISelectionHandler
{
private:
	int selectedIndex;
	std::vector<ISelectionHandler*> childrenSelectionHandlers;
	Size tabAreaSize;
	TabStrip* tabStrip;
	
	void DoSelectTabByIndex(int index);
public:
	TabPanel();
	virtual ~TabPanel();
	void OnInitialize();
	void OnUpdate();
	void OnDraw(Graphics& graphics, Position offset);
	void OnLayoutChange();
	virtual View* Copy();

	virtual bool SetProperty(std::string name, std::string value);
	void AddChildView(View* view);

	void HandleInput(InputState* input);
	int GetSelectedIndex();
	View* GetSelectedView();
	bool SelectByName(std::string name);
	bool SelectByIndex(int index);
	bool SelectById(std::string id);
	View* SelectNext(Direction direction);
};

/*********************************************/

#endif
