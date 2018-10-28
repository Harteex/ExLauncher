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

#ifndef _RECENT_APP_H_
#define _RECENT_APP_H_

/*********************************************/

#include <SDL.h>
#include <string>
#include <map>
#include <vector>
#include "App.h"

/*********************************************/

class RecentApp
{
private:
	App* app;
	std::string withFile;
public:
	RecentApp(App* app);
	RecentApp(App* app, std::string withFile);

	std::string GetAppId();
	App* GetApp();
	bool IsWithFile();
	std::string GetWithFilePath();
	void SetWithFilePath(std::string withFile);
};

/*********************************************/

#endif
