#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <vector>
#include <queue>
#include <chrono>
#include <ratio>
#include "SDL.h"

#include "Object.h"
#include "Tile.h"
#include "Player.h"
#include "Emerald.h"
#include "Gold.h"
#include "Enemy.h"

using namespace std::chrono;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

struct point
{
	point()
		:x(0)
		,y(0)
	{}

	point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	point operator+(const point& p2)
	{
		point temp(this->x + p2.x, this->y + p2.y);
		return temp;
	}

	bool operator!=(const point & p2)
	{
		return (x != p2.x && y != p2.y);
	}

	bool operator==(const point & p2)
	{
		return (x == p2.x && y == p2.y);
	}

	int x;
	int y;

	bool isValid() const
	{
		return (x >= 0 && x < WINDOW_HEIGHT / TILE_SIZE && y >= 0 && y < WINDOW_WIDTH / TILE_SIZE);
	}

	point operator*(size_t i)
	{
		point p3(x*i, y*i);
		return p3;
	}
};


class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	bool InitEverything();
	void RunGame();

private:

	// Setup
	bool InitSDL();
	bool CreateWindow();
	bool CreateRenderer();
	void SetupRenderer();
	void initTextures();
	void initBoard();
	void initStartPath(size_t x, size_t y, size_t num_paths, bool direction);
	void initPrimStartPath();
	void rec_initStartPath(point curr, size_t paths_num);
	void createEmeraldPatch(size_t left_x, size_t top_y, size_t width, size_t height);
	void initEmeralds(size_t num);
	void initCashBags(size_t num);
	void initGame();

	// helperFunctions
	SDL_Texture* LoadTexture(const std::string &str);
	bool noEnemies(int x, int y);
	bool noBags(int x, int y);

	// Update ( happens every frame )
	void Render();
	void HandleInput();
	void UpdateObjects();
	void spawnEnemies();
	void moveEnemies();
	void BFS(Enemy& enemy);
	void updateBags();

	SDL_Rect windowRect;
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool quit; //quit stops the game loop

	//time points
	time_point<high_resolution_clock> lastEnemySpawn;
	time_point< high_resolution_clock > lastEnemyMovement;
	time_point< high_resolution_clock > lastBagMovement;
	
	//Objects
	Player player;
	std::vector<Enemy> enemies;
	std::vector<CashBag> bags;
	std::vector<Gold> gold;

	//Textures
	SDL_Texture* dirt;
	SDL_Texture* steve;
	SDL_Texture* emerald;
	SDL_Texture* spider;
	SDL_Texture* bag;
	SDL_Texture* gold_ingot;

	//Board
	const size_t board_height;
	const size_t board_width;
	Tile* board[WINDOW_HEIGHT / TILE_SIZE][WINDOW_WIDTH / TILE_SIZE];

	//bonus variables
	double enemySpawnRate;
	double enemyMoveRate;
	double bagsFallingRate;
	size_t maxEnemies;
	size_t emeraldsNumber;
	

};

#endif