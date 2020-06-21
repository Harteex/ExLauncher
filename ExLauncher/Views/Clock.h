/*
Copyright 2020 Andreas Bjerkeholt

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

#ifndef _CLOCK_H_
#define _CLOCK_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "Label.h"
#include "../structures.h"
#include <vector>
#include <string>
#include "../Graphics/graphics_utils.h"

/*********************************************/

class Clock : public Label
{
private:
	time_t lastUpdatedTime;
	ClockType clockType;

	std::string GetTimeDateFormat();
public:
	Clock();
	~Clock();
	void OnUpdate();

	ClockType GetClockType();
	void SetClockType(ClockType clockType);
	bool SetProperty(std::string name, std::string value);
};

/*********************************************/

#endif
