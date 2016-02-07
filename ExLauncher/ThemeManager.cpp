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

#include "ThemeManager.h"
#include <sstream>

using namespace std;

string ThemeManager::curTheme = "default";

ThemeManager::ThemeManager()
{
}

ThemeManager::~ThemeManager()
{
}

string ThemeManager::ProcessPath(string path)
{
	stringstream ss;
	if (path.substr(0, 7) == "@theme/")
	{
		ss << "data/themes/" << curTheme << "/" << path.substr(7);
		return ss.str();
	}

	return path;
}
