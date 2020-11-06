#ifndef GOLD_H_INCLUDED
#define GOLD_H_INCLUDED

#include "Object.h"

class Gold : public Object
{
public:
	Gold(int x, int y);

	size_t getPoints() const;

private:
	size_t points;


};



#endif