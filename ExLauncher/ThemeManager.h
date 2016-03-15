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

#ifndef _THEME_MANAGER_H
#define _THEME_MANAGER_H

/*********************************************/

#include <string>
#include <map>
#include "Theme.h"

/*********************************************/

class ThemeManager
{
private:
	static std::string curTheme;

	std::map<std::string, Theme*> themes;
public:
	ThemeManager();
	~ThemeManager();

	static std::string ProcessPath(std::string path);
	static std::string GetCurrentThemeId();
	static void SetTheme(std::string themeId);
	static void LoadSettings();
	static void SaveSettings();

	void LoadThemes();
	void UnloadThemes();
	Theme* GetTheme(std::string id);
	std::map<std::string, Theme*> GetAllThemes();
};

#endif
