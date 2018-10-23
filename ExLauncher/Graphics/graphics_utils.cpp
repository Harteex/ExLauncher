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

#include "graphics_utils.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <SDL_image.h>
#include "../utils.h"
#include <map>
#include "../global.h"
#include <algorithm>

using namespace std;

SDL_Surface* _CreateEmptySurface(int width, int height)
{
	SDL_Surface *surface;
	Uint32 rmask, gmask, bmask, amask;

	// SDL interprets each pixel as a 32-bit number, so our masks must depend on the endianness (byte order) of the machine
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	surface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
	return surface;
}

Rectangle ClipRect(Rectangle clip, Rectangle origClip)
{
	int newX, newY, newX2, newY2;
	newX = clip.x < origClip.x ? origClip.x : clip.x;
	newY = clip.y < origClip.y ? origClip.y : clip.y;
	newX2 = (clip.x + clip.w) >(origClip.x + origClip.w) ? (origClip.x + origClip.w) : (clip.x + clip.w);
	newY2 = (clip.y + clip.h) >(origClip.y + origClip.h) ? (origClip.y + origClip.h) : (clip.y + clip.h);

	int newW, newH;
	newW = max(newX2 - newX, 0);
	newH = max(newY2 - newY, 0);

	return Rectangle(newX, newY, newW, newH);
}

map<string, Color> colorMap;

void InitColorMap()
{
	if (!colorMap.empty())
		return;

	colorMap["aliceblue"] = Color(0xfff0f8ff);
	colorMap["antiquewhite"] = Color(0xfffaebd7);
	colorMap["aqua"] = Color(0xff00ffff);
	colorMap["aquamarine"] = Color(0xff7fffd4);
	colorMap["azure"] = Color(0xfff0ffff);
	colorMap["beige"] = Color(0xfff5f5dc);
	colorMap["bisque"] = Color(0xffffe4c4);
	colorMap["black"] = Color(0xff000000);
	colorMap["blanchedalmond"] = Color(0xffffebcd);
	colorMap["blue"] = Color(0xff0000ff);
	colorMap["blueviolet"] = Color(0xff8a2be2);
	colorMap["brown"] = Color(0xffa52a2a);
	colorMap["burlywood"] = Color(0xffdeb887);
	colorMap["cadetblue"] = Color(0xff5f9ea0);
	colorMap["chartreuse"] = Color(0xff7fff00);
	colorMap["chocolate"] = Color(0xffd2691e);
	colorMap["coral"] = Color(0xffff7f50);
	colorMap["cornflowerblue"] = Color(0xff6495ed);
	colorMap["cornsilk"] = Color(0xfffff8dc);
	colorMap["crimson"] = Color(0xffdc143c);
	colorMap["cyan"] = Color(0xff00ffff);
	colorMap["darkblue"] = Color(0xff00008b);
	colorMap["darkcyan"] = Color(0xff008b8b);
	colorMap["darkgoldenrod"] = Color(0xffb8860b);
	colorMap["darkgray"] = Color(0xffa9a9a9);
	colorMap["darkgrey"] = Color(0xffa9a9a9);
	colorMap["darkgreen"] = Color(0xff006400);
	colorMap["darkkhaki"] = Color(0xffbdb76b);
	colorMap["darkmagenta"] = Color(0xff8b008b);
	colorMap["darkolivegreen"] = Color(0xff556b2f);
	colorMap["darkorange"] = Color(0xffff8c00);
	colorMap["darkorchid"] = Color(0xff9932cc);
	colorMap["darkred"] = Color(0xff8b0000);
	colorMap["darksalmon"] = Color(0xffe9967a);
	colorMap["darkseagreen"] = Color(0xff8fbc8f);
	colorMap["darkslateblue"] = Color(0xff483d8b);
	colorMap["darkslategray"] = Color(0xff2f4f4f);
	colorMap["darkslategrey"] = Color(0xff2f4f4f);
	colorMap["darkturquoise"] = Color(0xff00ced1);
	colorMap["darkviolet"] = Color(0xff9400d3);
	colorMap["deeppink"] = Color(0xffff1493);
	colorMap["deepskyblue"] = Color(0xff00bfff);
	colorMap["dimgray"] = Color(0xff696969);
	colorMap["dimgrey"] = Color(0xff696969);
	colorMap["dodgerblue"] = Color(0xff1e90ff);
	colorMap["firebrick"] = Color(0xffb22222);
	colorMap["floralwhite"] = Color(0xfffffaf0);
	colorMap["forestgreen"] = Color(0xff228b22);
	colorMap["fuchsia"] = Color(0xffff00ff);
	colorMap["gainsboro"] = Color(0xffdcdcdc);
	colorMap["ghostwhite"] = Color(0xfff8f8ff);
	colorMap["gold"] = Color(0xffffd700);
	colorMap["goldenrod"] = Color(0xffdaa520);
	colorMap["gray"] = Color(0xff808080);
	colorMap["grey"] = Color(0xff808080);
	colorMap["green"] = Color(0xff008000);
	colorMap["greenyellow"] = Color(0xffadff2f);
	colorMap["honeydew"] = Color(0xfff0fff0);
	colorMap["hotpink"] = Color(0xffff69b4);
	colorMap["indianred"] = Color(0xffcd5c5c);
	colorMap["indigo"] = Color(0xff4b0082);
	colorMap["ivory"] = Color(0xfffffff0);
	colorMap["khaki"] = Color(0xfff0e68c);
	colorMap["lavender"] = Color(0xffe6e6fa);
	colorMap["lavenderblush"] = Color(0xfffff0f5);
	colorMap["lawngreen"] = Color(0xff7cfc00);
	colorMap["lemonchiffon"] = Color(0xfffffacd);
	colorMap["lightblue"] = Color(0xffadd8e6);
	colorMap["lightcoral"] = Color(0xfff08080);
	colorMap["lightcyan"] = Color(0xffe0ffff);
	colorMap["lightgoldenrodyellow"] = Color(0xfffafad2);
	colorMap["lightgray"] = Color(0xffd3d3d3);
	colorMap["lightgrey"] = Color(0xffd3d3d3);
	colorMap["lightgreen"] = Color(0xff90ee90);
	colorMap["lightpink"] = Color(0xffffb6c1);
	colorMap["lightsalmon"] = Color(0xffffa07a);
	colorMap["lightseagreen"] = Color(0xff20b2aa);
	colorMap["lightskyblue"] = Color(0xff87cefa);
	colorMap["lightslategray"] = Color(0xff778899);
	colorMap["lightslategrey"] = Color(0xff778899);
	colorMap["lightsteelblue"] = Color(0xffb0c4de);
	colorMap["lightyellow"] = Color(0xffffffe0);
	colorMap["lime"] = Color(0xff00ff00);
	colorMap["limegreen"] = Color(0xff32cd32);
	colorMap["linen"] = Color(0xfffaf0e6);
	colorMap["magenta"] = Color(0xffff00ff);
	colorMap["maroon"] = Color(0xff800000);
	colorMap["mediumaquamarine"] = Color(0xff66cdaa);
	colorMap["mediumblue"] = Color(0xff0000cd);
	colorMap["mediumorchid"] = Color(0xffba55d3);
	colorMap["mediumpurple"] = Color(0xff9370db);
	colorMap["mediumseagreen"] = Color(0xff3cb371);
	colorMap["mediumslateblue"] = Color(0xff7b68ee);
	colorMap["mediumspringgreen"] = Color(0xff00fa9a);
	colorMap["mediumturquoise"] = Color(0xff48d1cc);
	colorMap["mediumvioletred"] = Color(0xffc71585);
	colorMap["midnightblue"] = Color(0xff191970);
	colorMap["mintcream"] = Color(0xfff5fffa);
	colorMap["mistyrose"] = Color(0xffffe4e1);
	colorMap["moccasin"] = Color(0xffffe4b5);
	colorMap["navajowhite"] = Color(0xffffdead);
	colorMap["navy"] = Color(0xff000080);
	colorMap["oldlace"] = Color(0xfffdf5e6);
	colorMap["olive"] = Color(0xff808000);
	colorMap["olivedrab"] = Color(0xff6b8e23);
	colorMap["orange"] = Color(0xffffa500);
	colorMap["orangered"] = Color(0xffff4500);
	colorMap["orchid"] = Color(0xffda70d6);
	colorMap["palegoldenrod"] = Color(0xffeee8aa);
	colorMap["palegreen"] = Color(0xff98fb98);
	colorMap["paleturquoise"] = Color(0xffafeeee);
	colorMap["palevioletred"] = Color(0xffdb7093);
	colorMap["papayawhip"] = Color(0xffffefd5);
	colorMap["peachpuff"] = Color(0xffffdab9);
	colorMap["peru"] = Color(0xffcd853f);
	colorMap["pink"] = Color(0xffffc0cb);
	colorMap["plum"] = Color(0xffdda0dd);
	colorMap["powderblue"] = Color(0xffb0e0e6);
	colorMap["purple"] = Color(0xff800080);
	colorMap["red"] = Color(0xffff0000);
	colorMap["rosybrown"] = Color(0xffbc8f8f);
	colorMap["royalblue"] = Color(0xff4169e1);
	colorMap["saddlebrown"] = Color(0xff8b4513);
	colorMap["salmon"] = Color(0xfffa8072);
	colorMap["sandybrown"] = Color(0xfff4a460);
	colorMap["seagreen"] = Color(0xff2e8b57);
	colorMap["seashell"] = Color(0xfffff5ee);
	colorMap["sienna"] = Color(0xffa0522d);
	colorMap["silver"] = Color(0xffc0c0c0);
	colorMap["skyblue"] = Color(0xff87ceeb);
	colorMap["slateblue"] = Color(0xff6a5acd);
	colorMap["slategray"] = Color(0xff708090);
	colorMap["slategrey"] = Color(0xff708090);
	colorMap["snow"] = Color(0xfffffafa);
	colorMap["springgreen"] = Color(0xff00ff7f);
	colorMap["steelblue"] = Color(0xff4682b4);
	colorMap["tan"] = Color(0xffd2b48c);
	colorMap["teal"] = Color(0xff008080);
	colorMap["thistle"] = Color(0xffd8bfd8);
	colorMap["tomato"] = Color(0xffff6347);
	colorMap["turquoise"] = Color(0xff40e0d0);
	colorMap["violet"] = Color(0xffee82ee);
	colorMap["wheat"] = Color(0xfff5deb3);
	colorMap["white"] = Color(0xffffffff);
	colorMap["whitesmoke"] = Color(0xfff5f5f5);
	colorMap["yellow"] = Color(0xffffff00);
	colorMap["yellowgreen"] = Color(0xff9acd32);
}

Color ParseColor(string colorStr)
{
	if (colorStr[0] == '#')
	{
		if (colorStr.length() == 7 || colorStr.length() == 9)
		{
			unsigned int color = strtoul(colorStr.substr(1).c_str(), NULL, 16);

			if (colorStr.length() == 7)
				color += 0xff000000;

			return Color(color);
		}
	}
	else
	{
		InitColorMap();

		auto knownColorResult = colorMap.find(colorStr);
		if (knownColorResult != colorMap.end())
			return knownColorResult->second;
	}

	return Color();
}