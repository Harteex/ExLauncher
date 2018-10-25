#include "FilesystemWatchListener.h"
#include <iostream>
#include "../ScreenSystem/ScreenManager.h"
#include "../App/AppManager.h"

FilesystemWatchListener::FilesystemWatchListener()
{
}

FilesystemWatchListener::~FilesystemWatchListener()
{
}

void FilesystemWatchListener::handleFileAction(FW::WatchID watchid, const FW::String & dir, const FW::String & filename, FW::Action action)
{
	std::cout << "DIR (" << dir + ") FILE (" + filename + ") has event " << action << std::endl;
}

void FilesystemWatchListener::SetScreenManager(ScreenManager* screenManager)
{
	this->screenManager = screenManager;
}

void FilesystemWatchListener::SetAppManager(AppManager* appManager)
{
	this->appManager = appManager;
}
