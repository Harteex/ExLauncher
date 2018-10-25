#include "FilesystemWatchManager.h"

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
	FW::WatchID watchID = fileWatcher.addWatch("/media/data/apps", &listener);
	if (watchID >= 0)
		watchesCount++;
#endif

	return true;
}

void FilesystemWatchManager::Update()
{
	if (watchesCount > 0)
		fileWatcher.update();
}
