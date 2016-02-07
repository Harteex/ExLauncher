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

#include "Image.h"
#include "../ThemeManager.h"
#include "../ScreenSystem/Screen.h"

using namespace std;

Image::Image()
{
	image = NULL;
	path = "";
	altPath = "";
}

Image::~Image()
{
}

bool Image::OnInitialize()
{
	image = context->GetResourceManager()->LoadImage(path, path.c_str());
	if (image == NULL)
	{
		image = context->GetResourceManager()->LoadImage(altPath, altPath.c_str());
		if (image == NULL)
			return false;
	}

	SDL_QueryTexture(image, NULL, NULL, &contentSize.w, &contentSize.h);
	
	return true;
}

void Image::OnDraw(SDL_Renderer* renderer, Position offset)
{
	if (image == NULL)
		return;

	SDL_Rect r;
	r.x = absolutePosition.x + offset.x;
	r.y = absolutePosition.y + offset.y;
	r.w = calculatedSize.w;
	r.h = calculatedSize.h;
	drawTexture(&r, image, renderer);
}

View* Image::Copy()
{
	Image* view = new Image();

	CopyBase(view);
	view->SetPath(path);
	view->SetAltPath(altPath);

	return view;
}

void Image::SetPath(string path)
{
	this->path = path;
}

void Image::SetAltPath(string path)
{
	this->altPath = path;
}

bool Image::SetProperty(string name, string value)
{
	bool propertyHandled = View::SetProperty(name, value);

	if (propertyHandled)
		return true;

	if (name == "path")
	{
		SetPath(ThemeManager::ProcessPath(value));
		return true;
	}
	else if (name == "altPath")
	{
		SetAltPath(ThemeManager::ProcessPath(value));
		return true;
	}

	return false;
}

void Image::FillData(map<string, string>& data)
{
	path = FindAndReplace(path, data);
	altPath = FindAndReplace(altPath, data);

	if (IsInitialized())
	{
		image = context->GetResourceManager()->LoadImage(path, path.c_str());
		if (image == NULL)
			image = context->GetResourceManager()->LoadImage(altPath, altPath.c_str());

		if (image != NULL)
			SDL_QueryTexture(image, NULL, NULL, &contentSize.w, &contentSize.h);

		RecalculateLayout();
	}
}
