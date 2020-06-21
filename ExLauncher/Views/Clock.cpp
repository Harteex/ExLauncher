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

#include "Clock.h"
#include <chrono>
#include <ctime>
#include <SDL.h>
#include "../Graphics/graphics_utils.h"
#include <sstream>
#include <limits.h>
#include "../ScreenSystem/Screen.h"
#include "../ViewSystem/LayoutHelper.h"
#include "../Graphics/GaussianBlur.h"
#include <algorithm>

using namespace std;

Clock::Clock()
{
	clockType = ClockTypeTimeShort;
	lastUpdatedTime = 0;
}

Clock::~Clock()
{
}

void Clock::OnUpdate()
{
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());

    if (now > lastUpdatedTime)
    {
		// TODO might not be worth updating every second unless clock type is long time (includes seconds).

        char bufTimeDate[100] = { 0 };
		string format = GetTimeDateFormat();
        strftime(bufTimeDate, sizeof(bufTimeDate), format.c_str(), localtime(&now));
        SetText(bufTimeDate);

        lastUpdatedTime = now;
    }
}

string Clock::GetTimeDateFormat()
{
	string time24HShort = "%H:%M";
	string time24HLong = "%H:%M:%S";
	string timeAmPmShort = "%I:%M %p";
	string timeAmPmLong = "%I:%M:%S %p";

	// TODO these should be read from settings
	bool time24H = true;
	string dateFormatShort = "%Y-%m-%d";
	string dateFormatLong = "%B %d, %Y";

	switch (clockType)
	{
	case ClockTypeTimeShort:
		return time24H ? time24HShort : timeAmPmShort;
	case ClockTypeTimeLong:
		return time24H ? time24HLong : timeAmPmLong;
	case ClockTypeDateShort:
		return dateFormatShort;
	case ClockTypeDateLong:
		return dateFormatLong;
		break;
	default:
		return string();
	}
}

ClockType Clock::GetClockType()
{
	return clockType;
}

void Clock::SetClockType(ClockType clockType)
{
	this->clockType = clockType;
}

bool Clock::SetProperty(string name, string value)
{
	bool propertyHandled = Label::SetProperty(name, value);

	if (propertyHandled)
		return true;

	if (name == "type")
	{
		if (value == "timeShort")
			SetClockType(ClockTypeTimeShort);
		else if (value == "timeLong")
			SetClockType(ClockTypeTimeLong);
		else if (value == "dateShort")
			SetClockType(ClockTypeDateShort);
		else if (value == "dateLong")
			SetClockType(ClockTypeDateLong);
		else
			throw runtime_error("invalid type value");

		return true;
	}

	return false;
}
