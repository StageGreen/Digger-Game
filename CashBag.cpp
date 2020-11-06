#include <iostream>

#include "CashBag.h"

CashBag::CashBag(int x, int y)
	:falling(false)
	,active(false)
	,numOfFallenTiles(0)
	,timeUntilFall(0.7)
{
	SetPos(x, y);
	time_point< high_resolution_clock > lastBagMovement = high_resolution_clock::now();
}

void CashBag::setFalling(bool fall)
{
	falling = fall;
}

void CashBag::setActive(bool _active)
{
	active = _active;
	if(active)
		fallingActive = high_resolution_clock::now();
}

bool CashBag::isFalling() const
{
	return falling;
}

bool CashBag::isActive() const
{
	return active;
}

time_point< high_resolution_clock > CashBag::getTimer() const
{
	return fallingActive;
}

int CashBag::getFallenTiles() const
{
	return numOfFallenTiles;
}

void CashBag::incrementFallenTiles()
{
	numOfFallenTiles++;
}

double CashBag::getTimeUntilFall() const
{
	return timeUntilFall;
}

time_point< high_resolution_clock > CashBag::getLastBagMovement() const
{
	return lastBagMovement;
}

void CashBag::resetFallTimer()
{
	lastBagMovement = high_resolution_clock::now();
}