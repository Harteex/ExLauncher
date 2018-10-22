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

#ifndef _IMAGE_H_
#define _IMAGE_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include <vector>
#include <string>
#include "../Graphics/graphics_utils.h"

/*********************************************/

class Image : public View
{
private:
	SDL_Texture* image;
	std::string path;
	std::string altPath;
public:
	Image();
	~Image();
	void OnInitialize();
	void OnDraw(Graphics& graphics, Position offset);
	View* Copy();

	void SetPath(std::string path);
	void SetAltPath(std::string path);
	bool SetProperty(std::string name, std::string value);
	void FillData(std::map<std::string, std::string>& data);
};

/*********************************************/

#endif
