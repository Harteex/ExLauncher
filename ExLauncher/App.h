/*
Copyright 2016 Andreas Bjerkeholt

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

#ifndef _APP_H_
#define _APP_H_

/*********************************************/

#include <SDL.h>
#include <string>
#include <map>
#include <vector>

/*********************************************/

class App
{
private:
	std::map<std::string, std::string> data;
	std::vector<std::string> exec;
public:
	/*std::string name;
	std::string target;
	SDL_Texture* icon;
	std::string description;
	std::string manual;*/

	std::map<std::string, std::string>& GetAllData();
	std::string GetData(std::string key, std::string defaultValue);
	void SetData(std::string key, std::string value);

	std::vector<std::string> GetExec();
	void SetExec(std::vector<std::string> exec);

	App();
};

/*********************************************/

#endif
