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

#ifndef _APP_MANAGER_H
#define _APP_MANAGER_H

/*********************************************/

#include <SDL.h>
#include "App.h"
#include "RecentApp.h"
#include <string>
#include <vector>
#include <map>
#include <vector>
#include <list>
#include "../ResourceManager.h"

/*********************************************/

class AppManager
{
private:
	std::map<std::string, std::vector<App*>*> appsInCategories;
	std::map<std::string, App*> apps;
	std::vector<RecentApp*> recentApps;
	ResourceManager* resourceManager;
	std::vector<std::string> commandToLaunch;

	std::vector<App*>* FindOrCreateCategory(std::string category);
	std::vector<App*>* FindOrCreateCategory(std::string category, std::map<std::string, std::vector<App*>*>* categoryMap);
	void AddApp(App* app);
	void RemoveApp(std::string path);
	void FindAndRemoveAppFromRecent(std::string appId);
	void FindAndRemoveAppFromRecent(std::string appId, std::string withFile);
	void LoadAllAppsXml();
	void LoadAppXml(std::string path);
#ifdef HAS_LIBOPK
	bool ListOpks(std::vector<std::string>& listToFill, std::string path);
	App* ParseOpkMetadata(struct OPK *opk);
	bool LoadOpk(std::string path);
#endif
public:
	AppManager();
	~AppManager();
		
	void SetResourceManager(ResourceManager* resourceManager);
	bool LoadApps();
	void UnloadApps();
	void HandleAppChange(std::string path, FileStatus status);
	std::map<std::string, std::vector<App*>*>* GetAllAppsWithCategoryMap();
	std::list<std::tuple<std::string, App*>> GetAppsByPathWithCategoryList(std::string path);
	App* GetApp(std::string id);
	std::vector<App*>* GetApps(std::string category);
	std::vector<App*> GetAppsByPath(std::string path);
	int GetNumberOfApps();
	bool SetAppToLaunch(std::string appId, std::string withFile, std::vector<std::string> command);
	void SetCommandToLaunch(std::vector<std::string> command);
	std::vector<std::string> GetCommandToLaunch();
	void ClearCommandToLaunch();

	bool LoadRecentList();
	bool SaveRecentList();
	RecentApp* GetRecent(int recentIndex);
};

#endif
