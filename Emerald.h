#ifndef EMERALD_H_INCLUDED
#define EMERALD_H_INCLUDED

#include "Object.h"

const size_t emerald_points = 10;

class Emerald : public Object
{
public:
	Emerald();
	Emerald(int x, int y);
	size_t getPoints() const; 



private:
	size_t points;


};


#endif