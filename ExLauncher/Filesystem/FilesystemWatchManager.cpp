#include "FilesystemWatchManager.h"

FilesystemWatchManager::FilesystemWatchManager()
{
}

FilesystemWatchManager::~FilesystemWatchManager()
{
}

bool FilesystemWatchManager::Init(ScreenManager* screenManager, AppManager* appManager)
{
	listener.SetScreenManager(screenManager);
	listener.SetAppManager(appManager);

	FW::WatchID watchID = fileWatcher.addWatch("/media/data/apps", &listener); // FIXME path?

	return true;
}

void FilesystemWatchManager::Update()
{
	fileWatcher.update();
}
