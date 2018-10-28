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

#include "RecentApp.h"

using namespace std;

RecentApp::RecentApp(App* app)
{
	this->app = app;
	this->withFile = "";
}

RecentApp::RecentApp(App* app, std::string withFile)
{
	this->app = app;
	this->withFile = withFile;
}

string RecentApp::GetAppId()
{
	if (app == nullptr)
		return "";

	return app->GetData("id", "");
}

App * RecentApp::GetApp()
{
	return app;
}

bool RecentApp::IsWithFile()
{
	return !withFile.empty();
}

string RecentApp::GetWithFilePath()
{
	return withFile;
}

void RecentApp::SetWithFilePath(string withFile)
{
	this->withFile = withFile;
}
