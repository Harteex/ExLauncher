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
	std::map<std::string, std::vector<App*>*> apps;
	ResourceManager* resourceManager;
	std::vector<std::string> commandToLaunch;

	std::vector<App*>* FindOrCreateCategory(std::string category);
	void AddApp(App* app);
public:
	AppManager();
	~AppManager();
		
	void SetResourceManager(ResourceManager* resourceManager);
	bool LoadApps();
	void UnloadApps();
	std::map<std::string, std::vector<App*>*>* GetAllApps();
	std::vector<App*>* GetApps(std::string category);
	void SetCommandToLaunch(std::vector<std::string> command);
	std::vector<std::string> GetCommandToLaunch();
	void ClearCommandToLaunch();
};

#endif
