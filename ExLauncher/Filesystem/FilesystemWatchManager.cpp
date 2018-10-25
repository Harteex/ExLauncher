#include "FilesystemWatchManager.h"

using namespace std;

FilesystemWatchManager::FilesystemWatchManager()
{
	watchesCount = 0;
}

FilesystemWatchManager::~FilesystemWatchManager()
{
}

bool FilesystemWatchManager::Init(ScreenManager* screenManager, AppManager* appManager)
{
	listener.SetScreenManager(screenManager);
	listener.SetAppManager(appManager);

#ifdef PLATFORM_GCW0
	string watchPath = "/media/data/apps";
	std::cout << "Setting up watch for path '" << watchPath << "'... ";
	FW::WatchID watchID = fileWatcher.addWatch(watchPath.c_str(), &listener);
	if (watchID >= 0)
	{
		watchesCount++;
		std::cout << "OK" << std::endl;
	}
	else
	{
		std::cout << "Failed" << std::endl;
	}
#endif

	return true;
}

void FilesystemWatchManager::Update()
{
	if (watchesCount > 0)
		fileWatcher.update();
}
