#include <iostream>

#include "SDL.h"
#include "Tile.h"

Tile::Tile()
	:_isDirt(true)
	,emerald(nullptr)
{}

Tile::Tile(int x, int y)
	:_isDirt(true)
	,emerald(nullptr)
{
	SetPos(x, y);
}

Tile::~Tile()
{
	if(hasEmerald())
		delete emerald;
}

void Tile::Render(SDL_Renderer* renderer)
{
	if (_isDirt)
	{
		Object::Render(renderer);
		if(hasEmerald())
			emerald->Render(renderer);
	}
}

void Tile::digOut()
{
	_isDirt = false;
	if (hasEmerald())
		delete emerald;
	emerald = nullptr;
}

void Tile::setEmerald(Emerald * emerald)
{
	this->emerald = emerald;
}

bool Tile::hasEmerald() const
{
	return (emerald != nullptr);
}

bool Tile::emptyDirt() const
{
	return (emerald == nullptr && _isDirt);
}

bool Tile::isDirt() const
{
	return _isDirt;
}
