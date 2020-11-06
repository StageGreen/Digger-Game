#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "SDL.h"

const int TILE_SIZE = 40;



class Object
{
public:
	Object();
	~Object() = default;

	// Initializes a texture, including size and position
	//void LoadTexture(SDL_Renderer *renderer, const std::string &str);
	void Render(SDL_Renderer* renderer);

	void SetRect(SDL_Rect r);
	void SetRect(int x, int y, int w, int h);
	void SetPos(int x, int y);
	void SetTexture(SDL_Texture* texture);

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	SDL_Rect GetRect() const;
	int getX() const; 
	int getY() const;

protected:
	int boardX;
	int boardY;
	SDL_Rect rect;

	SDL_Texture* texture;
};


#endif
