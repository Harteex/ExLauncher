#ifndef _APP_MANAGER_H
#define _APP_MANAGER_H

/*********************************************/

#include <SDL.h>
#include "App.h"
#include <string>
#include <vector>
#include <map>
#include "ResourceManager.h"

/*********************************************/

class AppManager
{
private:
	std::map<std::string, std::vector<App*>*> appsInCategories;
	std::map<std::string, App*> apps;
	std::vector<App*> recentApps;
	ResourceManager* resourceManager;
	std::vector<std::string> commandToLaunch;

	std::vector<App*>* FindOrCreateCategory(std::string category);
	void AddApp(App* app);
	void FindAndRemoveAppFromRecent(std::string appId);
public:
	AppManager();
	~AppManager();
		
	void SetResourceManager(ResourceManager* resourceManager);
	bool LoadApps();
	void UnloadApps();
	std::map<std::string, std::vector<App*>*>* GetAllAppsWithCategoryMap();
	std::vector<App*>* GetApps(std::string category);
	int GetNumberOfApps();
	bool SetAppToLaunch(std::string appId, std::vector<std::string> command);
	void SetCommandToLaunch(std::vector<std::string> command);
	std::vector<std::string> GetCommandToLaunch();
	void ClearCommandToLaunch();

	bool LoadRecentList();
	bool SaveRecentList();
	App* GetRecent(int recentIndex);
};

#endif
