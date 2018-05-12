#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS.h"
#include "Components.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

extern Manager manager;
time_t t;



Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts)
{
	scaledSize = ms * ts;
}


Map::~Map()
{
	
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
	tile.addGroup(Game::groupMap);
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	

	int srcX, srcY;

	// Random Generator Variables
	int appleCheck;
	srand((unsigned)time(&t));

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			srcY = atoi(&c) * tileSize;
			mapFile.get(c);
			srcX = atoi(&c) * tileSize;
			AddTile(srcX, srcY, x * (scaledSize), y * (scaledSize));
			mapFile.ignore();
		}
	}
	mapFile.ignore();

	

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			if ((y != 0) && (y != sizeY -1) && (x != 0) && (x != sizeX -1) && (y != 1 || x != 1))
				
					{
						int appleCheck = (rand() % 2);
						if (appleCheck == 0)
						{
							Game::assets->CreateBomb(Vector2D(x * (scaledSize), y * (scaledSize)), "bomb");

						}
					}
				
			mapFile.get(c);
			if (c == '1')
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders); 
				
				
				

			}

			mapFile.ignore();
		}
	}


	mapFile.close();
}



