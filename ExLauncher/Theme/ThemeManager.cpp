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
#include <fstream>
#include "../utils.h"
#include <iostream>
#include "../Filesystem/HomeDirectory.h"

using namespace std;

string ThemeManager::curTheme = "exlauncher";

ThemeManager::ThemeManager()
{
}

ThemeManager::~ThemeManager()
{
	UnloadThemes();
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

std::string ThemeManager::GetCurrentThemeId()
{
	return curTheme;
}

void ThemeManager::SetTheme(std::string themeId)
{
	curTheme = themeId;
}

void ThemeManager::LoadSettings()
{
	string configPath = HomeDirectory::GetConfigPath();
	string recentFile = configPath + "/theme.cfg";

	ifstream infile(recentFile);
	std::string line;
	if (std::getline(infile, line))
	{
		// FIXME make sure we have loaded the theme, if not use default
		curTheme = line;
	}
}

void ThemeManager::SaveSettings()
{
	string configPath = HomeDirectory::GetConfigPath();
	string recentFile = configPath + "/theme.cfg";

	ofstream outfile(recentFile, ios::out);
	outfile << curTheme;
}

void ThemeManager::LoadThemes()
{
	UnloadThemes();

	cout << "Loading themes:" << std::endl;

	vector<string> themeIds = getDirectories("data/themes/");
	for (string themeId : themeIds)
	{
		Theme* theme = new Theme(themeId);

		try
		{
			theme->LoadTheme();
		}
		catch (exception& ex)
		{
			cout << "FAILED TO LOAD " << themeId << std::endl;

			delete theme;
			continue;
		}

		cout << theme->GetName() << " (" << themeId << ")" << std::endl;
		themes[themeId] = theme;
	}
}

void ThemeManager::UnloadThemes()
{
	for (auto entry : themes)
	{
		delete entry.second;
	}

	themes.clear();
}

Theme * ThemeManager::GetTheme(std::string id)
{
	auto search = themes.find(id);
	if (search != themes.end())
	{
		return search->second;
	}
	else
	{
		return nullptr;
	}
}

map<string, Theme*> ThemeManager::GetAllThemes()
{
	return themes;
}
