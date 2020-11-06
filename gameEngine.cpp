#include <iostream>
#include <chrono>
#include "SDL.h"
#include <SDL_image.h>

#include "gameEngine.h"

using namespace std::chrono;
using namespace std;

GameEngine::GameEngine()
	:quit(false)
	,board_height(WINDOW_HEIGHT / TILE_SIZE)
	,board_width(WINDOW_WIDTH / TILE_SIZE)
	, enemySpawnRate(5) //enemies spawn every 5 seconds
	, enemyMoveRate (0.5) //enemies move every 0.5 seconds
	, bagsFallingRate(0.3) //bags fall every 0.3 seconds
	, maxEnemies(3)
	, emeraldsNumber(0)
{
	windowRect = { 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT }; // x y width height
}

GameEngine::~GameEngine()
{
	for (size_t i = 0; i < board_height; i++)
	{
		for (size_t j = 0; j < board_width; j++)
		{
			delete board[i][j];
		}
	}
}

bool GameEngine::InitEverything()
{
	if (!InitSDL())
		return false;

	if (!CreateWindow())
		return false;

	if (!CreateRenderer())
		return false;

	SetupRenderer();

	return true;
}
bool GameEngine::InitSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}
bool GameEngine::CreateWindow()
{
	//window = SDL_CreateWindow( "Server", posX, posY, sizeX, sizeY, 0 );
	window = SDL_CreateWindow("Digger", windowRect.x, windowRect.y, windowRect.w, windowRect.h, 0);

	if (window == nullptr)
	{
		std::cout << "Failed to create window : " << SDL_GetError();
		return false;
	}

	return true;
}
bool GameEngine::CreateRenderer()
{
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == nullptr)
	{
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return false;
	}

	return true;
}
void GameEngine::SetupRenderer()
{
	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize(renderer, windowRect.w, windowRect.h);

	// Set color of renderer to brown
	SDL_SetRenderDrawColor(renderer, 101, 67, 33, 0);
}

// Initializes a texture, including size and position
SDL_Texture* GameEngine::LoadTexture(const std::string &str)
{
	SDL_Surface* surface = IMG_Load(str.c_str());

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	return texture;
}

void GameEngine::RunGame()
{
	initGame();
	while (!quit)
	{
		if (emeraldsNumber == 0)
			quit = true;

		HandleInput();
		UpdateObjects();

		Render();
		//SDL_delay(16);
	}
}

void GameEngine::Render()
{
	// Clear the window and make it all brown
	SDL_RenderClear(renderer);
	
	//Render board
	for (size_t i = 0; i < board_height; i++)  
	{
		for (size_t j = 0; j < board_width; j++)
		{
			board[i][j]->Render(renderer);
		}
	}
	
	player.Render(renderer);

	for (auto enemy : enemies)
	{
		enemy.Render(renderer);
	}
	for (auto bag : bags)
	{
		bag.Render(renderer);
	}
	for (auto ingot : gold)
	{
		ingot.Render(renderer);
	}

	// Render the changes above
	SDL_RenderPresent(renderer);
}

void GameEngine::HandleInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			quit = true;
		else if (event.type == SDL_KEYDOWN)
		{
			int p_x = player.getX();
			int p_y = player.getY();
			switch (event.key.keysym.sym)
			{
			case SDLK_RIGHT:
				if (player.getY() < board_width - 1 && noBags(p_x, p_y + 1))
				{
					player.MoveRight();
					if (board[player.getX()][player.getY()]->hasEmerald())
						emeraldsNumber--;
					board[player.getX()][player.getY()]->digOut();
				}
				break;
			case SDLK_LEFT:
				if (player.getY() > 0 && noBags(p_x, p_y - 1))
				{
					player.MoveLeft();
					if (board[player.getX()][player.getY()]->hasEmerald())
						emeraldsNumber--;
					board[player.getX()][player.getY()]->digOut();
				}
				break;
			case SDLK_DOWN:
				if (player.getX() < board_height - 1 && noBags(p_x + 1, p_y ))
				{
					player.MoveDown();
					if (board[player.getX()][player.getY()]->hasEmerald())
						emeraldsNumber--;
					board[player.getX()][player.getY()]->digOut();
				}
				break;
			case SDLK_UP:
				if (player.getX() > 0 && noBags(p_x - 1, p_y))
				{
					player.MoveUp();
					if (board[player.getX()][player.getY()]->hasEmerald())
						emeraldsNumber--;
					board[player.getX()][player.getY()]->digOut();
				}
				break;
			default:
				break;
			}
		}
	}
}

void GameEngine::UpdateObjects()
{
	spawnEnemies();
	moveEnemies();
	updateBags();
}

void GameEngine::initTextures()
{
	dirt = LoadTexture("Textures/dirt_block.png");
	steve = LoadTexture("Textures/player.png");
	emerald = LoadTexture("Textures/emerald.png");
	spider = LoadTexture("Textures/spider.png");
	bag = LoadTexture("Textures/cashbag.png");
	gold_ingot = LoadTexture("Textures/gold_ingot.png");
}

void GameEngine::initPrimStartPath()
{
	size_t path_len = 20;

	point direction[4] = { point(0,1), point(1,0), point(-1,0), point(0,-1) };

	std::vector<point> neighbours;
	bool visited[WINDOW_HEIGHT / TILE_SIZE][WINDOW_WIDTH / TILE_SIZE];
	memset(visited, false, sizeof(visited));


	point start(0, board_width - 1);

	board[start.x][start.y]->digOut();
	visited[start.x][start.y] = true;

	for (size_t i = 0, paths_chosen = 0; i < 4 && paths_chosen < 3; i++)
	{
		point curr = start + direction[i];
		if (curr.isValid())
		{
			point temp;
			for (size_t j = 0; j < 3; j++)
			{
				temp = point(curr + direction[i] * j);
				if (temp.isValid() && !visited[temp.x][temp.y])
				{
					board[temp.x][temp.y]->digOut();
					visited[temp.x][temp.y] = true;
				}
			}
			for (size_t q = 0; q < 4; q++)
			{
				point temp2(temp + direction[q]);
				if (temp2.isValid())
				{
					neighbours.push_back(temp2);
				}
			}
		}
	}

	for (size_t i = 0; i < path_len; i++)
	{
		if (neighbours.size() == 0)
			break;

		int j = rand() % neighbours.size();
		point curr = neighbours[j];
		neighbours.erase(neighbours.begin() + j);
		board[curr.x][curr.y]->digOut();
		visited[curr.x][curr.y] = true;
		for (size_t k = 0; k < 4; k++)
		{
			point temp;
			for (size_t j = 0; j < 3; j++)
			{
				temp = point(curr + direction[i] * j);
				if (temp.isValid())
				{
					board[temp.x][temp.y]->digOut();
					visited[temp.x][temp.y] = true;
				}
			}
			for (size_t q = 0; q < 4; q++)
			{
				point temp2(temp + direction[q]);
				if (temp2.isValid() && !visited[temp2.x][temp2.y])
				{
					neighbours.push_back(temp2);
				}
			}/*
			point curr_neighbour = curr + direction[k];
			if (curr_neighbour.isValid() && !visited[curr_neighbour.x][curr_neighbour.y])
			{
				size_t size = neighbours.size();
				for (size_t s = 0; s < size; s++)
				{
					if (neighbours[s] == curr_neighbour)
						break;
					if(s == size - 1 && (neighbours[s] != curr_neighbour))
						neighbours.push_back(curr_neighbour);

				}
			}*/
		}
	}
}

void GameEngine::rec_initStartPath(point curr, size_t paths_num)
{
	point direction[4] = { point(0,1), point(1,0), point(-1,0), point(0,-1) };

	if (paths_num == 0)
		return;

	for (size_t i = 0; i < 3; i++)
	{
		point temp;
		int dir;
		do { 
			dir = rand() % 4;
			point temp2(curr + direction[dir]);
			temp = temp2;
		} while (!temp.isValid());
		
		int len = rand() % 10;
		//point next(curr + direction[dir]);
		for (size_t i = 0; i < len && curr.isValid(); i++)
		{
			board[curr.x][curr.y]->digOut();
			curr = curr + direction[dir];
		}
		rec_initStartPath(curr, paths_num - 1);
	}
}

void GameEngine::initStartPath(size_t x, size_t y, size_t num_paths, bool direction = false) 
{
	if (num_paths == 0 || x < 0 || y > board_width)
		return;
	
	
	//Deciding the length of the path & direction (left or down)
	if(direction)
		direction = false;
	else 
		direction = true;
	
	size_t path_len; //we will make left paths a bit longer than downwards, because width > height

	if(direction) //if going left
		path_len = 5 + (rand() % 5);
	else //if going down
		path_len = 3 + (rand() % 5);

	for (size_t i = 0; i < path_len; i++)
	{
		if (direction)
			if (y - i > 0)
				board[x][y - i]->digOut();
			else
				return;
		else
			if (x + i < board_height)
				board[x + i][y]->digOut();
			else
				return;
		//Render();
		//SDL_Delay(200);  code used for animating path digging
	}

	if (direction)
	{
		initStartPath(x, y - path_len, num_paths - 1, direction);
	}
	else
	{
		initStartPath(x + path_len, y, num_paths - 1, direction);
	}
}

void GameEngine::createEmeraldPatch(size_t left_x, size_t top_y, size_t width, size_t height)
{
	//creating patch
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			int x = i + top_y;
			int y = j + left_x;

			if (!board[x][y]->hasEmerald() && board[x][y]->isDirt())
			{
				try {
					Emerald * new_em = new Emerald(x, y);
					new_em->SetRect(y * TILE_SIZE, x * TILE_SIZE, TILE_SIZE, TILE_SIZE);
					new_em->SetTexture(emerald);
					board[x][y]->setEmerald(new_em);
					emeraldsNumber++;
				}
				catch (std::bad_alloc&)
				{
					std::cerr << "Couldnt allocate memory";
				}
			}
		}
	}
}

void GameEngine::initEmeralds(size_t num)
{
	for (size_t i = 0; i < num; i++) //create num patches of emeralds
	{
		//determining location of patch
		size_t left_x = rand() % (board_width - 1 - 2); //max 2 width of an emerald patch
		size_t top_y = rand() % (board_height - 1 - 6); //max 6 height of an emerald patch
		size_t width = 2; //always 2
		size_t height; //4 or 6
		if (rand() % 2)
			height = 4;
		else
			height = 6;
		createEmeraldPatch(left_x, top_y, width, height);
	}
}

void GameEngine::initCashBags(size_t num)
{
	bool spawning = true;
	for (size_t i = 0; i < board_height -1 && spawning; i++)
	{
		for (size_t j = 0; j < board_width && spawning; j++)
		{
			if (num == 0)
				spawning = false;
			else if (!board[i][j]->hasEmerald() && board[i + 1][j]->hasEmerald())
			{
				bool spawn = rand() % 2;
				if (spawn)
				{
					CashBag new_bag(i, j);
					new_bag.SetRect(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
					new_bag.SetTexture(bag);
					//board[i][j]->setCashBag(new_bag);
					bags.push_back(new_bag);
					num--;
				}
			}
		}
	}
	while (num > 0)
	{
		int x = rand() % (board_height - 1);
		int y = rand() % (board_width - 1);
		if (board[x][y]->emptyDirt())
		{
			CashBag new_bag(x, y);
			new_bag.SetRect(y * TILE_SIZE, x * TILE_SIZE, TILE_SIZE, TILE_SIZE);
			new_bag.SetTexture(bag);
			//board[x][y]->setCashBag(new_bag);
			bags.push_back(new_bag);
			num--;
		}
	}
}

void GameEngine::initBoard()
{
	//initalize board array & dirt tiles
	for (size_t i = 0; i < board_height; i++)   
	{
		for (size_t j = 0; j < board_width; j++)
		{
			try {
				board[i][j] = new Tile(j, i);
				board[i][j]->SetRect(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
				board[i][j]->SetTexture(dirt);
			}
			catch (std::bad_alloc&)
			{
				std::cerr << "Couldnt allocate memory";
			}
		}
	}
	
	initStartPath(0, board_width - 1, 6); //make a maximum of 6 turns when building the path; start from upper right corner
	//initPrimStartPath();
	//rec_initStartPath(point(0, board_width - 1), 8);
	initEmeralds(6); //make 6 emerald patches
	initCashBags(8);

	//init Player
	player.SetPos(0, board_width - 1);
	player.SetRect((board_width - 1) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE);
	player.SetTexture(steve);


}

void GameEngine::spawnEnemies()
{
	if (enemies.size() >= maxEnemies)
		return;
	auto current = high_resolution_clock::now();
	duration< double > delta(current - lastEnemySpawn);
	if (delta.count() > enemySpawnRate)
	{
		enemies.push_back(Enemy(0, board_width - 1));
		enemies[enemies.size() - 1].SetRect((board_width - 1)*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE);
		enemies[enemies.size() - 1].SetTexture(spider);
		lastEnemySpawn = current;
	}
}

void GameEngine::moveEnemies()
{
	auto current = high_resolution_clock::now();
	duration< double > delta(current - lastEnemyMovement);
	if (delta.count() > enemyMoveRate)
	{
		for (auto& enemy : enemies)
		{
			BFS(enemy);
			if (enemy.getX() == player.getX() && enemy.getY() == player.getY())
				quit = true;
		}
		lastEnemyMovement = current;
		
	}
}

void GameEngine::BFS(Enemy& enemy)
{
	int e_x = enemy.getX();
	int e_y = enemy.getY();
	int p_x = player.getX();
	int p_y = player.getY();

	if (e_x == p_x && e_y == p_x) //enemy is already on top of player
		return;

	point direction[4] = {point(0,1), point(1,0), point(-1,0), point(0,-1)}; 

	bool visited[WINDOW_HEIGHT / TILE_SIZE][WINDOW_WIDTH / TILE_SIZE];
	memset(visited, false, sizeof(visited));

	point parent[WINDOW_HEIGHT / TILE_SIZE][WINDOW_WIDTH / TILE_SIZE];
	memset(parent, -1, sizeof(parent));

	std::queue<point> Q;
	Q.push(point(e_x, e_y));
	visited[e_x][e_y] = true;

	point curr;
	bool playerFound = false;
	while (!Q.empty() && !playerFound)
	{
		curr = Q.front();
		Q.pop();
		
		for (size_t i = 0; i < 4; i++)
		{
			point next = curr + direction[i];
			if (next.isValid() && !board[next.x][next.y]->isDirt() && !visited[next.x][next.y]) 
			{
				visited[next.x][next.y] = true;
				parent[next.x][next.y] = curr;
				Q.push(next);

				if (next.x == p_x && next.y == p_y) //if we found the player stop
				{
					curr = next; //for name purposes
					playerFound = true;
					break;
				}
			}
		}
	}
	
	if (curr.x == p_x && curr.y == p_y) //traceback the path to the enemy
	{
		point next(curr);
		while (next.x != e_x || next.y != e_y)
		{
			curr = next;
			next = parent[next.x][next.y];
		}
	}

	if (noEnemies(curr.x, curr.y))
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (e_x + direction[i].x == curr.x && e_y + direction[i].y == curr.y) //identify direction
				switch (i)
				{
				case 0: enemy.MoveRight(); break;
				case 1: enemy.MoveDown(); break;
				case 2: enemy.MoveUp(); break;
				case 3: enemy.MoveLeft(); break;
				}

		}
	}
}

void GameEngine::updateBags()
{
	for (size_t i = 0; i < bags.size(); i++)
	{
		int x = bags[i].getX();
		int y = bags[i].getY();
		
		if (!bags[i].isActive())
		{
			if (!board[x + 1][y]->isDirt())
				bags[i].setActive(true);
		}
		if (bags[i].isActive() && !bags[i].isFalling())
		{
			auto current = high_resolution_clock::now();
			duration< double > delta(current - bags[i].getTimer());
			if (delta.count() > bags[i].getTimeUntilFall())
				bags[i].setFalling(true);
		}
		if (bags[i].isFalling())
		{
			auto current = high_resolution_clock::now();
			duration< double > delta(current - bags[i].getLastBagMovement());
			if (delta.count() > bagsFallingRate)
			{
				if (x + 1 == board_height - 1 || (board[x + 1][y]->isDirt() && bags[i].getFallenTiles() > 1)) //break bag and let that sweet gold out
				{
					//spawn gold ingot
					gold.push_back(Gold(x, y));
					gold[gold.size() - 1].SetRect(x*TILE_SIZE, y, TILE_SIZE, TILE_SIZE);
					gold[gold.size() - 1].SetTexture(gold_ingot);
					//bags[i].resetFallTimer();
					//lastBagMovement = high_resolution_clock::now();
					bags.erase(bags.begin() + i);
				}
				else if (!board[x + 1][y]->isDirt())
				{
					bags[i].MoveDown();
					bags[i].incrementFallenTiles();
					if (player.getX() == x && player.getY() == y)
						quit = true;
					for (int i = 0; i < enemies.size(); i++)
					{
						int e_x = enemies[i].getX();
						int e_y = enemies[i].getY();
						if (e_x == x && e_y == y || e_x + 1 == x && e_y + 1 == y)
							enemies.erase(enemies.begin() + i);
					}
					bags[i].resetFallTimer();
					//lastBagMovement = high_resolution_clock::now();
				}
			}
		}
	}
}

bool GameEngine::noEnemies(int x, int y)
{
	for (auto enemy : enemies)
	{
		if (enemy.getX() == x && enemy.getY() == y)
			return false;
	}
	return true;
}

bool GameEngine::noBags(int x, int y)
{
	for (auto bag : bags)
	{
		if (bag.getX() == x && bag.getY() == y)
			return false;
	}
	return true;
}

void GameEngine::initGame()
{
	initTextures();
	initBoard();
	lastEnemySpawn = high_resolution_clock::now();
	lastEnemyMovement = high_resolution_clock::now();
	lastBagMovement = high_resolution_clock::now();

}
