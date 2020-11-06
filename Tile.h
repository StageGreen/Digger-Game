#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include "Object.h"
#include "Emerald.h"
#include "CashBag.h"

class Tile : virtual public Object
{
public:
	Tile();
	Tile(int x, int y);
	~Tile();
	void Render(SDL_Renderer* renderer);
	void digOut();
	void setEmerald(Emerald * emerald);
	bool hasEmerald() const;
	bool emptyDirt() const;
	bool isDirt() const;

private:
	bool _isDirt;
	Emerald * emerald;
	CashBag * bag;
};


#endif
