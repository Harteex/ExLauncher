#ifndef _APP_MANAGER_H
#define _APP_MANAGER_H

/*********************************************/

#include <SDL.h>
#include "App.h"
#include <string>
#include <vector>
#include <map>

/*********************************************/

class AppManager
{
private:
	std::map<std::string, std::vector<App*>*> apps;

	std::vector<App*>* FindOrCreateCategory(std::string category);
	void AddApp(App* app);
public:
	AppManager();
	~AppManager();
		
	bool LoadApps();
	std::map<std::string, std::vector<App*>*>* GetAllApps();
	std::vector<App*>* GetApps(std::string category);
};

#endif
