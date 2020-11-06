#include <iostream>

#include "Player.h"


Player::Player()
	:points(0)
{}

Player::Player(int x, int y)
	:points(0)
{
	SetPos(x, y);
}

void Player::addPoints(size_t points)
{
	this->points += points;
}

size_t Player::getPoints() const
{
	return points;
}

void Player::Render(SDL_Renderer * renderer)
{
	Object::Render(renderer);
}
