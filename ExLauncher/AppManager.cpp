#include "AppManager.h"

using namespace std;

AppManager::AppManager()
{
}

AppManager::~AppManager()
{
}

bool AppManager::LoadApps()
{
#ifdef HAS_LIBOPK
	// OPK CODE
#else
	// If no libopk, for example when compiling and testing with Visual Studio, just load example data

	std::vector<App*>* categoryGames = new std::vector<App*>();
	std::vector<App*>* categoryApplications = new std::vector<App*>();
	std::vector<App*>* categorySettings = new std::vector<App*>();
	std::vector<App*>* categoryEmulators = new std::vector<App*>();

	App* appGame1 = new App();
	appGame1->SetData("name", "SameGoo");
	categoryGames->push_back(appGame1);
	App* appGame2 = new App();
	appGame2->SetData("name", "UMG");
	categoryGames->push_back(appGame2);
	App* appGame3 = new App();
	appGame3->SetData("name", "Jet Set Radio Future");
	categoryGames->push_back(appGame3);
	App* appGame4 = new App();
	appGame4->SetData("name", "Halo");
	categoryGames->push_back(appGame4);
	App* appGame5 = new App();
	appGame5->SetData("name", "Crysis");
	categoryGames->push_back(appGame5);
	App* appGame6 = new App();
	appGame6->SetData("name", "Final Fantasy");
	categoryGames->push_back(appGame6);
	App* appGame7 = new App();
	appGame7->SetData("name", "Crash Bandicoot");
	categoryGames->push_back(appGame7);
	App* appGame8 = new App();
	appGame8->SetData("name", "Crackdown");
	categoryGames->push_back(appGame8);
	App* appGame9 = new App();
	appGame9->SetData("name", "Minecraft");
	categoryGames->push_back(appGame9);
	App* appGame10 = new App();
	appGame10->SetData("name", "Counter Strike");
	categoryGames->push_back(appGame10);
	App* appGame11 = new App();
	appGame11->SetData("name", "Just Cause 2");
	categoryGames->push_back(appGame11);
	App* appGame12 = new App();
	appGame12->SetData("name", "The Witcher");
	categoryGames->push_back(appGame12);
	App* appGame13 = new App();
	appGame13->SetData("name", "Age of Empires");
	categoryGames->push_back(appGame13);
	App* appGame14 = new App();
	appGame14->SetData("name", "Team Fortress 2");
	categoryGames->push_back(appGame14);
	App* appGame15 = new App();
	appGame15->SetData("name", "Halflife 3");
	categoryGames->push_back(appGame15);
	App* appGame16 = new App();
	appGame16->SetData("name", "Settlers 2");
	categoryGames->push_back(appGame16);
	App* appGame17 = new App();
	appGame17->SetData("name", "Dead or Alive");
	categoryGames->push_back(appGame17);
	App* appGame18 = new App();
	appGame18->SetData("name", "Amped");
	categoryGames->push_back(appGame18);

	App* app4 = new App();
	app4->SetData("name", "Console");
	categoryApplications->push_back(app4);

	App* app5 = new App();
	app5->SetData("name", "Wireless");
	categorySettings->push_back(app5);
	App* app6 = new App();
	app6->SetData("name", "Shutdown");
	categorySettings->push_back(app6);

	App* app7 = new App();
	app7->SetData("name", "Snes9x");
	categorySettings->push_back(app7);

	apps["games"] = categoryGames;
	apps["applications"] = categoryApplications;
	apps["settings"] = categorySettings;
	apps["emulators"] = categoryEmulators;
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
