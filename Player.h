#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Object.h" 

class Player : virtual public Object
{
public:
	Player();
	Player(int x, int y);

	void addPoints(size_t points);
	size_t getPoints() const;

	void Render(SDL_Renderer * renderer);

private:
	size_t points;

};


#endif