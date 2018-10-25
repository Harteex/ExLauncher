/*
Copyright 2018 Andreas Bjerkeholt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _FILESYSTEM_WATCH_LISTENER_H
#define _FILESYSTEM_WATCH_LISTENER_H

/*********************************************/

#include <SDL.h>
#include "SimpleFileWatcher/FileWatcher.h"
#include <string>

/*********************************************/

class ScreenManager;
class AppManager;

class FilesystemWatchListener : public FW::FileWatchListener
{
private:
	ScreenManager* screenManager;
	AppManager* appManager;
public:
	FilesystemWatchListener();
	~FilesystemWatchListener();
	void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action);
	void SetScreenManager(ScreenManager* screenManager);
	void SetAppManager(AppManager* appManager);
};

#endif
