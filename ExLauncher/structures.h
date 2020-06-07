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

#ifndef _STRUCTURES_H
#define _STRUCTURES_H

#include <string>

class NetplayManager;

struct Position
{
	int x;
	int y;

	Position() : x(0), y(0) {}
	Position(int x, int y) : x(x), y(y) {}
};

extern bool operator==(const Position& lhs, const Position& rhs);
extern Position operator+(const Position& lhs, const Position& rhs);
extern Position operator-(const Position& lhs, const Position& rhs);
extern Position operator-(const Position& lhs);

struct PositionF
{
	double x;
	double y;
};

struct Size
{
	int w;
	int h;

	Size() : w(0), h(0) {}
	Size(int w, int h) : w(w), h(h) {}
};

extern bool operator==(const Size& lhs, const Size& rhs);
extern Size operator+(const Size& lhs, const Size& rhs);
extern Size operator-(const Size& lhs, const Size& rhs);
extern Size operator-(const Size& lhs);

struct Box
{
	int top;
	int bottom;
	int left;
	int right;

	Box() : top(0), bottom(0), left(0), right(0) {}
	Box(int top, int bottom, int left, int right) : top(top), bottom(bottom), left(left), right(right) {}
};

struct Rectangle
{
	int x;
	int y;
	int w;
	int h;

	Rectangle() : x(0), y(0), w(0), h(0) {}
	Rectangle(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
};

#define BLACK 0xff000000
#define WHITE 0xffffffff

struct Color
{
	int r;
	int g;
	int b;
	int a;

	Color() : r(0), g(0), b(0), a(0xff) {}
	Color(int r, int g, int b) : r(r), g(g), b(b), a(0xff) {}
	Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}
	Color(unsigned int color) : r((color >> 16) & 0xff), g((color >> 8) & 0xff), b(color & 0xff), a((color >> 24) & 0xff) {}
};

enum Direction {DirectionLeft, DirectionRight, DirectionUp, DirectionDown};
enum Orientation {OrientationHorizontal, OrientationVertical};
enum SizeMode {SizeModeNormal, SizeModeCompact};
enum TransitionEffect {TransitionEffectZoom};
enum EventType {EventTypeAction, EventTypeGoBack, EventTypeSelectionChanged, EventTypeDataCollectionChanged};
enum TextStyle {TextStylePlain, TextStyleShadow};
enum LetterCase {LetterCaseOriginal, LetterCaseUpper, LetterCaseLower};
enum FileStatus {FileStatusAdded, FileStatusDeleted, FileStatusModified};

#define GRAVITY_LEFT 0
#define GRAVITY_TOP 0
#define GRAVITY_HCENTER 1
#define GRAVITY_RIGHT 2
#define GRAVITY_VCENTER 4
#define GRAVITY_BOTTOM 8

#endif
