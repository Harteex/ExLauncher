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

#include "AppManager.h"
#include "../utils.h"
#include <algorithm>
#include "../Filesystem/HomeDirectory.h"
#include "../RapidXml/rapidxml.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

#ifdef HAS_LIBOPK
#include <dirent.h>
#include <opk.h>
#endif

using namespace rapidxml;
using namespace std;

AppManager::AppManager()
{
	resourceManager = NULL;
	commandToLaunch = vector<string>();
}

AppManager::~AppManager()
{
	UnloadApps();
}

void AppManager::SetResourceManager(ResourceManager* resourceManager)
{
	this->resourceManager = resourceManager;
}

vector<App*>* AppManager::FindOrCreateCategory(string category)
{
	auto categoryResult = appsInCategories.find(category);
	if (categoryResult != appsInCategories.end())
		return categoryResult->second;

	vector<App*>* categoryNew = new vector<App*>();
	appsInCategories[category] = categoryNew;

	return categoryNew;
}

void AppManager::AddApp(App* app)
{
	// FIXME verify id not empty
	apps[app->GetData("id", "")] = app;

	string categoriesString = app->GetData("categories", "uncategorized");
	vector<string> categories = split(categoriesString, ';');
	
	for (string category : categories)
	{
		vector<App*>* categoryAppContainer = FindOrCreateCategory(category);
		categoryAppContainer->push_back(app);
	}
}

void AppManager::RemoveApp(std::string path)
{
	// Remove app from all category structures
	for (auto appCategory : appsInCategories)
	{
		vector<App*>* appsVector = appCategory.second;

		for (auto it = appsVector->begin(); it != appsVector->end(); )
		{
			if ((*it)->GetData("path", "") == path)
				it = appsVector->erase(it);
			else
				it++;
		}
	}

	// Remove app from all apps list, remove it from the recent list, and finally also free the app object
	for (auto it = apps.begin(); it != apps.end(); )
	{
		App* app = it->second;

		if (app->GetData("path", "") == path)
		{
			FindAndRemoveAppFromRecent(app->GetData("id", ""));

			it = apps.erase(it);
			delete app;
		}
		else
		{
			it++;
		}
	}
}

#ifdef HAS_LIBOPK
bool ListOpks(vector<string>& listToFill, string path)
{
	DIR* dir;
	struct dirent* dnt;

	dir = opendir(path.c_str());
	if (dir == NULL)
		return false;

	while (dnt = readdir(dir))
	{
		string fileName = dnt->d_name;

		if (fileName.length() < 5)
			continue;

		if (fileName.substr(fileName.length() - 4) != ".opk")
			continue;

		string fullPath = path + fileName;
		listToFill.push_back(fullPath);
	}

	closedir(dir);

	return true;
}

App* ParseOpkMetadata(struct OPK *opk)
{
	const char *key, *val;
	size_t keyLength, valLength;
	int ret;

	App* app = new App();

	while ((ret = opk_read_pair(opk, &key, &keyLength, &val, &valLength)))
	{
		if (ret < 0)
		{
			// TODO error that we could not read the metadata
			delete app;
			return NULL;
		}

		// Buffer value so that we get a null terminated string
		char valueBuffer[valLength + 1];
		sprintf(valueBuffer, "%.*s", valLength, val);

		if (strncmp(key, "Categories", keyLength) == 0)
		{
			app->SetData("categories", valueBuffer);
		}
		else if (strncmp(key, "Name", keyLength) == 0)
		{
			app->SetData("name", valueBuffer);
		}
		else if (strncmp(key, "Icon", keyLength) == 0)
		{
			app->SetData("iconName", valueBuffer);
		}

		// TODO handle more values

	}

	return app;
}

bool LoadOpk(string path)
{
	struct OPK *opk = opk_open(opkPath.c_str());
	if (!opk)
	{
		// TODO error
		return false;
	}

	while (true)
	{
		const char *metadataName;
		int ret = opk_open_metadata(opk, &metadataName);
		if (ret <= 0)
		{
			// TODO error that loading metadata failed?
			return false;
		}

		string name(metadataName);
		string platform = "";
		size_t pos1 = string::npos;
		size_t pos2 = string::npos;

		pos2 = name.rfind(".");
		if (pos2 != string::npos && pos2 > 0)
		{
			pos1 = name.rfind(".", pos2 - 1);
			if (pos1 != string::npos)
			{
				pos1++;
				platform = name.substr(pos1, pos2 - pos1);
			}
		}

		// FIXME PLATFORM gcw0 is hardcoded
		if (platform.compare("gcw0") == 0 || platform.compare("all") == 0)
		{
			// We have metadata to load
			App* app = ParseOpkMetadata(opk);
			if (app != NULL)
			{
				app->SetData("path", opkPath);
				app->SetData("metadata", metadataName);

				stringstream appId;
				appId << opkPath << " " << metadataName;
				app->SetData("id", appId.str());

				stringstream exec;
				exec << "opkrun -m " << metadataName << " " << opkPath;
				app->SetData("exec", exec.str());
				app->SetExec({ "opkrun", "-m", metadataName, opkPath });

				// FIXME do not try to load icon if none is specified
				string iconId = opkPath + "/" + app->GetData("iconName", "");
				app->SetData("iconId", iconId);

				SDL_Texture* iconTexture = resourceManager->LoadImageFromOpk(iconId, opk, (string)app->GetData("iconName", "") + ".png");
				if (iconTexture == nullptr)
					app->SetData("iconId", "appIconDefault");

				AddApp(app);
			}
		}
	}

	opk_close(opk);
	return true;
}
#endif

void AppManager::LoadAllAppsXml()
{
	vector<string> files = getFilesByExtension("data/apps/", "xml");
	for (string file : files)
	{
		string path = string("data/apps/") + file;
		LoadAppXml(path);
	}
}

void AppManager::LoadAppXml(string path)
{
	App* app = nullptr;

	try {
		xml_document<> doc;
		ifstream file(path.c_str());
		stringstream buffer;
		buffer << file.rdbuf();
		file.close();
		std::string content(buffer.str());

		if (content.size() == 0)
			throw runtime_error("not a valid xml file");

		doc.parse<0>(&content[0]);

		xml_node<> * startNode = NULL;
		startNode = doc.first_node();

		if (startNode == NULL || strcmp(startNode->name(), "App") != 0)
			throw runtime_error("not a valid app");

		app = new App();
		app->SetData("id", path);

		for (xml_node<> * node = startNode->first_node(); node; node = node->next_sibling())
		{
			string str = node->name();

			if (str == "Name")
				app->SetData("name", node->value());
			else if (str == "Comment")
				app->SetData("comment", node->value());
			else if (str == "Exec")
			{
				app->SetData("exec", node->value());
				app->SetExec({ node->value() });
			}
			else if (str == "Icon")
			{
				string iconId = node->value();
				app->SetData("iconId", iconId);

				SDL_Texture* iconTexture = resourceManager->LoadImage(iconId, iconId.c_str());
				if (iconTexture == nullptr)
					app->SetData("iconId", "appIconDefault");
			}
			else if (str == "Categories")
				app->SetData("categories", node->value());
		}

		AddApp(app);
	}
	catch (exception& ex)
	{
		if (app != nullptr)
			delete app;
	}
}

bool AppManager::LoadApps()
{
	// Create default categories (should always exist)
	FindOrCreateCategory("games");
	FindOrCreateCategory("applications");
	FindOrCreateCategory("settings");
	FindOrCreateCategory("emulators");

	LoadAllAppsXml();

#ifdef HAS_LIBOPK
	
	vector<string> opkPaths;
	ListOpks(opkPaths, "/media/data/apps/");

	for (string opkPath : opkPaths)
	{
		bool result = LoadOpk(opkPath);
		// FIXME show error if any fails?
	}

#else
	// If no libopk, for example when compiling and testing with Visual Studio, just load example data

	App* app;

	app = new App();
	app->SetData("id", "samegoo");
	app->SetData("name", "SameGoo");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	app->SetData("exec", "test");
	AddApp(app);

	app = new App();
	app->SetData("id", "umg");
	app->SetData("name", "UMG");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "jetsetradiofuture");
	app->SetData("name", "Jet Set Radio Future");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "halo");
	app->SetData("name", "Halo");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "crysis");
	app->SetData("name", "Crysis");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "finalfantasy");
	app->SetData("name", "Final Fantasy");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "crashbandicoot");
	app->SetData("name", "Crash Bandicoot");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "crackdown");
	app->SetData("name", "Crackdown");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "minecraft");
	app->SetData("name", "Minecraft");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "counterstrike");
	app->SetData("name", "Counter Strike");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "justcause2");
	app->SetData("path", "/path/to/justcause2.opk");
	app->SetData("name", "Just Cause 2");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "thewitcher");
	app->SetData("path", "/path/to/thewitcher.opk");
	app->SetData("name", "The Witcher");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "ageofempires");
	app->SetData("path", "/path/to/ageofempires.opk");
	app->SetData("name", "Age of Empires");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "teamfortress2");
	app->SetData("path", "/path/to/teamfortress2.opk");
	app->SetData("name", "Team Fortress 2");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "halflife3");
	app->SetData("name", "Halflife 3");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "settlers2");
	app->SetData("name", "Settlers 2");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "deadoralive");
	app->SetData("name", "Dead or Alive");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "amped");
	app->SetData("name", "Amped");
	app->SetData("categories", "games;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "console");
	app->SetData("name", "Console");
	app->SetData("categories", "applications;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "wireless");
	app->SetData("name", "Wireless");
	app->SetData("categories", "settings;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "shutdown");
	app->SetData("name", "Shutdown");
	app->SetData("categories", "settings;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "test1");
	app->SetData("name", "Test Uncategorized");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "test2");
	app->SetData("name", "Test Custom Category");
	app->SetData("categories", "customcategory;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);

	app = new App();
	app->SetData("id", "test3");
	app->SetData("name", "Test Multiple Categories");
	app->SetData("categories", "customcategory;settings;");
	app->SetData("iconId", "appIconDefault");
	AddApp(app);
#endif

	App* changeThemeApp = new App();
	changeThemeApp->SetData("id", "settingsChangeTheme");
	changeThemeApp->SetData("name", "Change Theme");
	changeThemeApp->SetData("categories", "settings;");
	changeThemeApp->SetData("iconId", "appIconDefault");
	changeThemeApp->SetExec({ "settingsChangeTheme" });
	AddApp(changeThemeApp);

	return true;
}

// FIXME just delete the apps from the apps map now that it exists, instead of extracting them from the appsInCategories map.
void AppManager::UnloadApps()
{
	// Move apps to a temporary vector before deleting, because an app may exist in multiple categories
	vector<App*> appsToDelete;

	// Find all apps, add them to the delete list, and free the map/vectors
	for (auto appCategory : appsInCategories)
	{
		vector<App*>* appsVector = appCategory.second;

		for (App* app : *appsVector)
		{
			if (std::find(appsToDelete.begin(), appsToDelete.end(), app) == appsToDelete.end())
			{
				// Not previously added to the remove list
				appsToDelete.push_back(app);
			}
		}

		(*appsVector).clear();

		delete appsVector;
	}

	appsInCategories.clear();
	apps.clear();

	// Now finally delete all the apps
	for (App* app : appsToDelete)
	{
		delete app;
	}

	appsToDelete.clear();
}

map<string, vector<App*>*>* AppManager::GetAllAppsWithCategoryMap()
{
	return &appsInCategories;
}

vector<App*>* AppManager::GetApps(string category)
{
	if (appsInCategories.count(category) == 0)
		return NULL;

	return appsInCategories[category];
}

int AppManager::GetNumberOfApps()
{
	return apps.size();
}

bool AppManager::SetAppToLaunch(std::string appId, std::vector<std::string> command)
{
	bool appFound = false;
	auto app = apps.find(appId);
	if (app != apps.end())
	{
		FindAndRemoveAppFromRecent(appId);
		recentApps.insert(recentApps.begin(), app->second);
		appFound = true;
	}

	this->commandToLaunch = command;

	return appFound;
}

void AppManager::SetCommandToLaunch(vector<string> command)
{
	this->commandToLaunch = command;
}

vector<string> AppManager::GetCommandToLaunch()
{
	return commandToLaunch;
}

void AppManager::ClearCommandToLaunch()
{
	commandToLaunch.clear();
}

bool AppManager::LoadRecentList()
{
	string configPath = HomeDirectory::GetConfigPath();
	string recentFile = configPath + "/recent.cfg";

	ifstream infile(recentFile);
	std::string line;
	while (std::getline(infile, line))
	{
		auto idFound = apps.find(line);
		if (idFound != apps.end())
		{
			recentApps.push_back(idFound->second);
		}
	}

	return true;
}

bool AppManager::SaveRecentList()
{
	string configPath = HomeDirectory::GetConfigPath();
	string recentFile = configPath + "/recent.cfg";
	ofstream outfile(recentFile, ios::out);

	for (auto it = recentApps.begin(); it != recentApps.end(); it++)
	{
		App* app = *it;
		outfile << app->GetData("id", "") << endl;
	}

	return true;
}

App* AppManager::GetRecent(int recentIndex)
{
	if (recentIndex < 0 || recentIndex >= recentApps.size())
		return nullptr;

	return recentApps.at(recentIndex);
}

void AppManager::FindAndRemoveAppFromRecent(string appId)
{
	if (appId == "")
		return;

	for (auto it = recentApps.begin(); it != recentApps.end(); it++)
	{
		if ((*it)->GetData("id", "") == appId)
		{
			recentApps.erase(it);
			break;
		}
	}
}
