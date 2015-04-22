#include "structures.h"

bool operator==(const Position& lhs, const Position& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

Position operator+(const Position& lhs, const Position& rhs)
{
	return Position(lhs.x + rhs.x, lhs.y + rhs.y);
}

Position operator-(const Position& lhs, const Position& rhs)
{
	return Position(lhs.x - rhs.x, lhs.y - rhs.y);
}

bool operator==(const Size& lhs, const Size& rhs)
{
	return lhs.w == rhs.w && lhs.h == rhs.h;
}

Size operator+(const Size& lhs, const Size& rhs)
{
	return Size(lhs.w + rhs.w, lhs.h + rhs.h);
}

Size operator-(const Size& lhs, const Size& rhs)
{
	return Size(lhs.w - rhs.w, lhs.h - rhs.h);
}
