#include "AppManager.h"
#include "utils.h"

#ifdef HAS_LIBOPK
#include <dirent.h>
#include <opk.h>
#endif

using namespace std;

AppManager::AppManager()
{
	resourceManager = NULL;
}

AppManager::~AppManager()
{
}

void AppManager::SetResourceManager(ResourceManager* resourceManager)
{
	this->resourceManager = resourceManager;
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

		// TODO handle more values, for example icon

	}

	return app;
}
#endif

bool AppManager::LoadApps()
{
	// Create default categories (should always exist)
	FindOrCreateCategory("games");
	FindOrCreateCategory("applications");
	FindOrCreateCategory("settings");
	FindOrCreateCategory("emulators");

#ifdef HAS_LIBOPK
	
	vector<string> opkPaths;
	ListOpks(opkPaths, "/media/data/apps/");

	for (string opkPath : opkPaths)
	{
		struct OPK *opk = opk_open(opkPath.c_str());
		if (!opk)
		{
			// TODO error
			continue;
		}

		while (true)
		{
			const char *metadataName;
			int ret = opk_open_metadata(opk, &metadataName);
			if (ret < 0)
			{
				// TODO error that loading metadata failed
				break;
			}
			else if (!ret)
			{
				break;
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
					// FIXME do not try to load icon if none is specified
					string iconId = opkPath + "/" + app->GetData("iconName", "");
					app->SetData("iconId", iconId);
					resourceManager->LoadImageFromOpk(iconId, opk, (string)app->GetData("iconName", "") + ".png");

					AddApp(app);
				}
			}
		}

		opk_close(opk);
	}

#else
	// If no libopk, for example when compiling and testing with Visual Studio, just load example data

	App* app;

	app = new App();
	app->SetData("name", "SameGoo");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "UMG");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Jet Set Radio Future");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Halo");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Crysis");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Final Fantasy");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Crash Bandicoot");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Crackdown");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Minecraft");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Counter Strike");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Just Cause 2");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "The Witcher");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Age of Empires");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Team Fortress 2");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Halflife 3");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Settlers 2");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Dead or Alive");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Amped");
	app->SetData("categories", "games;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Console");
	app->SetData("categories", "applications;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Wireless");
	app->SetData("categories", "settings;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Shutdown");
	app->SetData("categories", "settings;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Test Uncategorized");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Test Custom Category");
	app->SetData("categories", "customcategory;");
	app->SetData("iconId", "@theme/appIconDefault.png");
	AddApp(app);

	app = new App();
	app->SetData("name", "Test Multiple Categories");
	app->SetData("categories", "customcategory;settings;");
	app->SetData("iconId", "@theme/appIconDefault.png");
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
