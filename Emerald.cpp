#include <iostream>

#include "Emerald.h"

Emerald::Emerald()
	:points(emerald_points)
{}

Emerald::Emerald(int x, int y)
	: points(emerald_points)
{
	SetPos(y, x);
}

size_t Emerald::getPoints() const
{
	return points;
}
