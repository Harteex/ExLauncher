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

#ifndef _THEME_H_
#define _THEME_H_

/*********************************************/

#include <string>
#include <vector>

/*********************************************/

class Theme
{
private:
	std::string id;
	std::string name;
	std::string entryPoint;
	std::string author;
	std::string copyright;
	std::string license;
	std::vector<std::string> thirdPartyDeclarations;

public:
	Theme(std::string id);
	~Theme();

	void LoadTheme();

	std::string GetName();
	std::string GetEntryPoint();
	std::string GetAuthor();
	std::string GetCopyright();
	std::string GetLicense();
	std::vector<std::string> GetThirdPartyNotices();
};

#endif
