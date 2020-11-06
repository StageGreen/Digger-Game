#ifndef CASHBAG_H_INCLUDED
#define CASHBAG_H_INCLUDED

#include <chrono>
#include <ratio>
#include "Object.h"

using namespace std::chrono;


class CashBag : public Object
{
public:
	CashBag(int x, int y);
	void setFalling(bool fall);
	void setActive(bool _active);
	void incrementFallenTiles();
	int getFallenTiles() const;
	bool isFalling() const;
	bool isActive() const;
	time_point< high_resolution_clock > getTimer() const;
	time_point< high_resolution_clock > getLastBagMovement() const;
	void resetFallTimer();
	double getTimeUntilFall() const;
	

private:
	bool falling;
	bool active;
	int numOfFallenTiles;
	time_point< high_resolution_clock > fallingActive;
	time_point< high_resolution_clock > lastBagMovement;
	double timeUntilFall;

};

#endif
