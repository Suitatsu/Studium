#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "Assetmanager.h"
#include <time.h>
#include <sstream>

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0,0,800,640 };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& triggerbox(manager.addEntity());
auto& label(manager.addEntity());

int facing = 0;


#define MAXAPPLES 99



Game::Game()
{
}


Game::~Game()
{
}

void Game::faceDirection(int direction)
{
	facing = direction;
}

void placeApple(int apples[][2], char board[][25], int sizeX, int sizeY, int numberOfApples)
{
	bool mark[25 * 25];
	int i;
	memset(mark, false, sizeof(mark));


	for (i = 0; i < numberOfApples;)
	{
		int random = rand() % (sizeX * sizeY);
		int x = random % sizeX;
		int y = random % sizeY;
		if ((y != 0) && (y != sizeY - 1) && (x != 0) && (x != sizeX - 1) && (y != 1 || x != 1))
		{

			if (mark[random] == false)
			{

				apples[i][0] = x;
				apples[i][1] = y;
				Game::assets->createBomb(Vector2D(x * (64), y * (64)), "bomb");
				board[apples[i][0]][apples[i][1]] = '*';
				mark[random] = true;

				i++;
			}
		}
	}
}

void placeCake(char board[][25], int sizeX, int sizeY)
{
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			if ((y != 0) && (y != sizeY - 1) && (x != 0) && (x != sizeX - 1))
			{
				int count = Map::countAdjacentMines(x, y, board);
				switch (count)
				{
				case 1:
					Game::assets->createNumber(Vector2D(x * (64), y * (64)), "number1"); break;
				case 2:
					Game::assets->createNumber(Vector2D(x * (64), y * (64)), "number2"); break;
				case 3:
					Game::assets->createNumber(Vector2D(x * (64), y * (64)), "number3"); break;
				case 4:
					Game::assets->createNumber(Vector2D(x * (64), y * (64)), "number4"); break;
				case 5:
					Game::assets->createNumber(Vector2D(x * (64), y * (64)), "number5"); break;
				case 6:
					Game::assets->createNumber(Vector2D(x * (64), y * (64)), "number6"); break;
				case 7:
					Game::assets->createNumber(Vector2D(x * (64), y * (64)), "number7"); break;
				case 8:
					Game::assets->createNumber(Vector2D(x * (64), y * (64)), "number8"); break;
				default:
					break;
				}
			}
			if (board[x][y] == '*')
			{

				// Creating Cake on top of the Bomb
				Game::assets->createCakeWithTag(Vector2D(x * (64), y * (64)), "cake", "fakecake");

				// Creating Cake in the West of the Bomb
				if ((y != 0) && (y != sizeY - 1) && ((x - 1) != 0) && ((x - 1) != sizeX - 1) && ((x - 1) != 1 || y != 1))
				Game::assets->createCake(Vector2D((x - 1) * (64), y * (64)), "cake");

				// Creating Cake in the East of the Bomb
				if ((y != 0) && (y != sizeY - 1) && ((x + 1) != 0) && ((x + 1) != sizeX - 1) && ((x + 1) != 1 || y != 1))
				Game::assets->createCake(Vector2D((x + 1) * (64), y * (64)), "cake");
				
				// Creating Cake in the North of the Bomb
				if (((y - 1) != 0) && ((y - 1) != sizeY - 1) && (x != 0) && (x != sizeX - 1) && (x != 1 || (y - 1) != 1))
				Game::assets->createCake(Vector2D(x * (64), (y - 1) * (64)), "cake");

				// Creating Cake in the South of the Bomb
				if (((y + 1) != 0) && ((y + 1) != sizeY - 1) && (x != 0) && (x != sizeX - 1) && (x != 1 || (y + 1) != 1))
				Game::assets->createCake(Vector2D(x * (64), (y + 1) * (64)), "cake");

				// Creating Cake in the North-West of the Bomb
				if (((y - 1) != 0) && ((y - 1) != sizeY - 1) && ((x - 1) != 0) && ((x - 1) != sizeX - 1) && ((x - 1) != 1 || (y - 1) != 1))
				Game::assets->createCake(Vector2D((x - 1) * (64), (y - 1) * (64)), "cake");

				// Creating Cake in the South-West of the Bomb
				if (((y + 1) != 0) && ((y + 1) != sizeY - 1) && ((x - 1) != 0) && ((x - 1) != sizeX - 1) && ((x - 1) != 1 || (y + 1) != 1))
				Game::assets->createCake(Vector2D((x - 1) * (64), (y + 1) * (64)), "cake");

				// Creating Cake in the South-East of the Bomb
				if (((y + 1) != 0) && ((y + 1) != sizeY - 1) && ((x + 1) != 0) && ((x + 1) != sizeX - 1) && ((x + 1) != 1 || (y + 1) != 1))
				Game::assets->createCake(Vector2D((x + 1) * (64), (y + 1) * (64)), "cake");

				// Creating Cake in the North-East of the Bomb
				if (((y - 1) != 0) && ((y - 1) != sizeY - 1) && ((x + 1) != 0) && ((x + 1) != sizeX - 1) && ((x + 1) != 1 || (y - 1) != 1))
				Game::assets->createCake(Vector2D((x + 1) * (64), (y - 1) * (64)), "cake");

		
			}
		}
	}
}

void Game::init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		int flags = 0;
		if (fullscreen)
		{
			flags = SDL_WINDOW_FULLSCREEN;
		}
		std::cout << "SDL_Init No Error: " << SDL_GetError() << std::endl;
		
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window created" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created" << std::endl;
		}

		isRunning = true;
	}
	
	if (TTF_Init() == -1)
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}

	int numberOfApples = 20;
	int apples[MAXAPPLES][2];
	char board[25][25];
	int x, y;
	

	assets->AddTexture("terrain", "assets/terrain_ss.png");
	assets->AddTexture("player", "assets/Animations.png");
	assets->AddTexture("bomb", "assets/Apple.png");
	assets->AddTexture("cake", "assets/Cake.png");
	assets->AddTexture("hurtbox", "assets/ColTex.png");
	assets->AddTexture("number1", "assets/one.png");
	assets->AddTexture("number2", "assets/two.png");
	assets->AddTexture("number3", "assets/three.png");
	assets->AddTexture("number4", "assets/four.png");
	assets->AddTexture("number5", "assets/five.png");
	assets->AddTexture("number6", "assets/six.png");
	assets->AddTexture("number7", "assets/seven.png");
	assets->AddTexture("number8", "assets/eight.png");

	//assets->AddFont("arial", "assets/arial.ttf", 16);

	map = new Map("terrain", 2, 32);

	map->LoadMap("assets/map.map", 25, 14);
	placeApple(apples, board, 25, 14, numberOfApples);
	placeCake(board, 25, 14);
	
	//isRunning = playMinesweeperUtil(board, apples, x, y);
	
	triggerbox.addComponent<TransformComponent>(0, 0, 6, 6, 1);
	triggerbox.addComponent<SpriteComponent>("hurtbox");
	triggerbox.addComponent<ColliderComponent>("hurtbox");
	triggerbox.addGroup(Game::groupTriggerbox);
	

	player.addComponent<TransformComponent>(32 * 2, 32 * 2, 2);				// Givint Starting Position of Player
	player.addComponent<SpriteComponent>("player", true);					// Giving animation Path of Player
	player.addComponent<KeyboardController>();								// Add Keyboardmovement to Player
	player.addComponent<ColliderComponent>("player");						// Add Collider to the Player and assign the Tag "player" 
	player.addGroup(groupPlayers);
	
	//label.addComponent<UILabel>(10, 10, "Text String", "arial", SDL_Color{ 255,255,255,255 });
	
}





auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& bomb(manager.getGroup(Game::groupBomb));
auto& cake(manager.getGroup(Game::groupCake));
auto& numbers(manager.getGroup(Game::groupNumber));
auto& triggerboxs(manager.getGroup(Game::groupTriggerbox));





void Game::handleEvents()
{
	

	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

int Game::update()
{
	static int splash = 0;
	while (splash < 1)
	{
		splash =+ 1;
		return 3;
	}


	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	/*std::stringstream ss;
	ss << "player position " << playerPos;

	label.getComponent<UILabel>().setText(ss.str(), "arial");*/

	triggerbox.getComponent<TransformComponent>().position.x = 0;
	triggerbox.getComponent<TransformComponent>().position.y = 0;

	if (Game::event.type == SDL_KEYDOWN)
	{
		switch (Game::event.key.keysym.sym)
		{
		case SDLK_SPACE:

			switch (facing)
			{
			case 1: // Looking Up
				triggerbox.getComponent<TransformComponent>().position.x = player.getComponent<TransformComponent>().position.x + 26;
				triggerbox.getComponent<TransformComponent>().position.y = player.getComponent<TransformComponent>().position.y + 0;
				break;
			case 2: // Looking Down
				triggerbox.getComponent<TransformComponent>().position.x = player.getComponent<TransformComponent>().position.x + 26;
				triggerbox.getComponent<TransformComponent>().position.y = player.getComponent<TransformComponent>().position.y + 80;
				break;
			case 3: // Looking Left
				triggerbox.getComponent<TransformComponent>().position.x = player.getComponent<TransformComponent>().position.x - 26;
				triggerbox.getComponent<TransformComponent>().position.y = player.getComponent<TransformComponent>().position.y + 46;
				break;
			case 4: // Looking Right
				triggerbox.getComponent<TransformComponent>().position.x = player.getComponent<TransformComponent>().position.x + 80;
				triggerbox.getComponent<TransformComponent>().position.y = player.getComponent<TransformComponent>().position.y + 46;
				break;
			default:
				break;
			}


		default:
			break;
		}
	}



	manager.refresh();
	manager.update();

	for (auto& c : colliders) // Check map collider with player collider
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	for (auto& k : cake) // Check Cake collider with player collider
	{
		SDL_Rect cCol = k->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
		if (Collision::AABB(triggerbox.getComponent<ColliderComponent>().collider, k->getComponent<ColliderComponent>().collider))
		{

			if (k->getComponent<ColliderComponent>().tag == "cake")
			{
				std::cout << triggerbox.getComponent<ColliderComponent>().tag << " collided with " << k->getComponent<ColliderComponent>().tag << std::endl;

				k->destroy();
			}
			if (k->getComponent<ColliderComponent>().tag == "fakecake")
			{
				std::cout << triggerbox.getComponent<ColliderComponent>().tag << " collided with " << k->getComponent<ColliderComponent>().tag << std::endl;
				k->getComponent<SpriteComponent>().setTex("bomb");

			}
		}

	}


	camera.x = player.getComponent<TransformComponent>().position.x - 400;
	camera.y = player.getComponent<TransformComponent>().position.y - 320;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

	return 1;
}






void Game::render()
{
	SDL_RenderClear(renderer);
	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& n : numbers)
	{
		n->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& e : enemies)
	{
		e->draw();
	}
	/*for (auto& b : bomb)
	{
		b->draw();
	}*/
	for (auto& c : cake)
	{
		c->draw();
	}
	for (auto& h : triggerboxs)
	{
		h->draw();
	}
	
	label.draw();

	SDL_RenderPresent(renderer);
}


void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}





