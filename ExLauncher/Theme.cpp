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

#include "Theme.h"
#include "RapidXml/rapidxml.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace rapidxml;
using namespace std;

Theme::Theme(string id)
{
	this->id = id;
}

Theme::~Theme()
{
}

void Theme::LoadTheme()
{
	string path = string("data/themes/") + id + string("/theme.xml");

	xml_document<> doc;
	ifstream file(path.c_str());
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());

	if (content.size() == 0)
		throw runtime_error("not a valid xml file");

	doc.parse<0>(&content[0]);

	xml_node<> * startNode = NULL;
	startNode = doc.first_node();

	if (startNode == NULL || strcmp(startNode->name(), "Theme") != 0)
		throw runtime_error("not a valid theme");

	for (xml_node<> * node = startNode->first_node(); node; node = node->next_sibling())
	{
		string str = node->name();

		if (str == "Name")
			name = node->value();
		else if (str == "EntryPoint")
			entryPoint = node->value();
		else if (str == "Author")
			author = node->value();
		else if (str == "Copyright")
			copyright = node->value();
		else if (str == "License")
			license = node->value();
		else if (str == "ThirdPartyDeclarations")
		{
			for (xml_node<> * thirdPartyDeclNode = node->first_node(); thirdPartyDeclNode; thirdPartyDeclNode = thirdPartyDeclNode->next_sibling())
			{
				if (string(thirdPartyDeclNode->name()) == "ContentDeclaration")
				{
					thirdPartyDeclarations.push_back(thirdPartyDeclNode->value());
				}
			}
		}
	}
}

std::string Theme::GetName()
{
	return name;
}

std::string Theme::GetEntryPoint()
{
	return entryPoint;
}

std::string Theme::GetAuthor()
{
	return author;
}

std::string Theme::GetCopyright()
{
	return copyright;
}

std::string Theme::GetLicense()
{
	return license;
}

std::vector<std::string> Theme::GetThirdPartyNotices()
{
	return thirdPartyDeclarations;
}
