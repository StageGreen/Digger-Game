#include <iostream>

#include "Gold.h"

Gold::Gold(int x, int y)
{
	SetPos(x, y);
}

size_t Gold::getPoints() const
{
	return points;
}