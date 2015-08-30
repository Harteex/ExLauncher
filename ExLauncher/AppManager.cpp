#include "AppManager.h"
#include "utils.h"

#ifdef HAS_LIBOPK
#include <dirent.h>
#include <opk.h>
#endif

using namespace std;

AppManager::AppManager()
{
}

AppManager::~AppManager()
{
}

vector<App*>* AppManager::FindOrCreateCategory(string category)
{
	auto categoryResult = apps.find(category);
	if (categoryResult != apps.end())
		return categoryResult->second;

	vector<App*>* categoryNew = new vector<App*>();
	apps[category] = categoryNew;

	return categoryNew;
}

void AppManager::AddApp(App* app)
{
	string categoriesString = app->GetData("categories", "uncategorized");
	vector<string> categories = split(categoriesString, ';');
	
	for (string category : categories)
	{
		vector<App*>* categoryAppContainer = FindOrCreateCategory(category);
		categoryAppContainer->push_back(app);
	}
}

bool AppManager::LoadApps()
{
	// Create default categories (should always exist)
	FindOrCreateCategory("games");
	FindOrCreateCategory("applications");
	FindOrCreateCategory("settings");
	FindOrCreateCategory("emulators");

#ifdef HAS_LIBOPK
	// OPK CODE
#else
	// If no libopk, for example when compiling and testing with Visual Studio, just load example data

	App* app;

	app = new App();
	app->SetData("name", "SameGoo");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "UMG");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Jet Set Radio Future");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Halo");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Crysis");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Final Fantasy");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Crash Bandicoot");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Crackdown");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Minecraft");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Counter Strike");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Just Cause 2");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "The Witcher");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Age of Empires");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Team Fortress 2");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Halflife 3");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Settlers 2");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Dead or Alive");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Amped");
	app->SetData("categories", "games;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Console");
	app->SetData("categories", "applications;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Wireless");
	app->SetData("categories", "settings;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Shutdown");
	app->SetData("categories", "settings;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Test Uncategorized");
	AddApp(app);

	app = new App();
	app->SetData("name", "Test Custom Category");
	app->SetData("categories", "customcategory;");
	AddApp(app);

	app = new App();
	app->SetData("name", "Test Multiple Categories");
	app->SetData("categories", "customcategory;settings;");
	AddApp(app);
#endif

	return true;
}

map<string, vector<App*>*>* AppManager::GetAllApps()
{
	return &apps;
}

vector<App*>* AppManager::GetApps(string category)
{
	if (apps.count(category) == 0)
		return NULL;

	return apps[category];
}
