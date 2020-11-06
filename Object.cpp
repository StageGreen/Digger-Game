#include <iostream>
#include "SDL.h"
#include <SDL_image.h>

#include "Object.h"

Object::Object()
	: rect({ 0, 0, 0, 0 })
	,boardX(0)
	,boardY(0)
	,texture(nullptr)
{

}
/*
// Initializes a texture, including size and position
void Object::LoadTexture(SDL_Renderer *renderer, const std::string &str)
{
	SDL_Surface* surface = IMG_Load(str.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
}*/

void Object::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

SDL_Rect Object::GetRect() const
{
	return rect;
}

int Object::getX() const
{
	return boardX;
}

int Object::getY() const
{
	return boardY;
}

void Object::SetRect(SDL_Rect r)
{
	rect.x = r.x;
	rect.y = r.y;
	rect.w = r.w;
	rect.h = r.h;
}

void Object::SetRect(int x, int y, int w, int h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}

void Object::SetPos(int x, int y)
{
	boardX = x;
	boardY = y;
}

void Object::SetTexture(SDL_Texture* texture)
{
	this->texture = texture;
}

void Object::MoveLeft()
{
	boardY -= 1;
	rect.x -= TILE_SIZE;
}
void Object::MoveRight()
{
	boardY += 1;
	rect.x += TILE_SIZE;
}
void Object::MoveUp()
{
	boardX -= 1;
	rect.y -= TILE_SIZE;
}
void Object::MoveDown()
{
	boardX += 1;
	rect.y += TILE_SIZE;
}
