#include "FilesystemWatchListener.h"
#include <iostream>
#include "../ScreenSystem/ScreenManager.h"
#include "../App/AppManager.h"
#include "SimpleFileWatcher/FileWatcher.h"

using namespace std;

FilesystemWatchListener::FilesystemWatchListener()
{
	screenManager = NULL;
	appManager = NULL;
}

FilesystemWatchListener::~FilesystemWatchListener()
{
}

void FilesystemWatchListener::handleFileAction(FW::WatchID watchid, const FW::String & dir, const FW::String & filename, FW::Action action)
{
	std::cout << "DIR (" << dir + ") FILE (" + filename + ") has event " << action << std::endl;
	
	string path;
	path += dir;
	path += "/";
	path += filename;

	FileStatus status;
	string eventValue;
	switch (action)
	{
	case 1:
		status = FileStatusAdded;
		eventValue = "added";
		break;
	case 2:
		status = FileStatusDeleted;
		eventValue = "deleted";
		break;
	case 4:
		status = FileStatusModified;
		eventValue = "modified";
		break;
	default:
		return;
	}

	if (appManager != NULL)
		appManager->HandleAppChange(path, status);

	if (screenManager != NULL)
	{
		vector<string> args;
		args.push_back(path);
		screenManager->SendEvent(EventTypeDataCollectionChanged, eventValue, args);
	}
}

void FilesystemWatchListener::SetScreenManager(ScreenManager* screenManager)
{
	this->screenManager = screenManager;
}

void FilesystemWatchListener::SetAppManager(AppManager* appManager)
{
	this->appManager = appManager;
}
