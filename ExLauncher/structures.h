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

struct Box
{
	int top;
	int bottom;
	int left;
	int right;

	Box() : top(0), bottom(0), left(0), right(0) {}
	Box(int top, int bottom, int left, int right) : top(top), bottom(bottom), left(left), right(right) {}
};

enum Direction {DirectionLeft, DirectionRight, DirectionUp, DirectionDown};
enum Orientation {OrientationHorizontal, OrientationVertical};
enum SizeMode {SizeModeNormal, SizeModeCompact};
enum TransitionEffect {TransitionEffectZoom};

#define GRAVITY_LEFT 0
#define GRAVITY_TOP 0
#define GRAVITY_HCENTER 1
#define GRAVITY_RIGHT 2
#define GRAVITY_VCENTER 4
#define GRAVITY_BOTTOM 8

#endif
