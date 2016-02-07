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
	virtual ~FramePanel();
	void OnLayoutChange();
	virtual View* Copy();

	virtual bool SetProperty(std::string name, std::string value);
};

/*********************************************/

#endif
