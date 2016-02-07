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

Position operator-(const Position& lhs)
{
	return Position(-lhs.x, -lhs.y);
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

Size operator-(const Size& lhs)
{
	return Size(-lhs.w, -lhs.h);
}
