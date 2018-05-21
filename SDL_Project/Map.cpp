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
int appleCheck;


bool isApple(int row, int col, char board[][25])
{
	if (board[row][col] == '*')
		return true;
	else
		return false;
}



int Map::countAdjacentMines(int row, int col, char board[][25]) // kommt noch
{
	int count = 0;
	
	// North
	if (isApple(row - 1, col, board) == true)  
		count++;
	// South
	if (isApple(row + 1, col, board) == true)
		count++;
	// East
	if (isApple(row, col + 1, board) == true)
		count++;
	// West
	if (isApple(row, col - 1, board) == true)
		count++;
	// North-East
	if (isApple(row - 1, col + 1, board) == true)
		count++;
	// North-West
	if (isApple(row - 1, col - 1, board) == true)
		count++;
	// South-East
	if (isApple(row + 1, col + 1, board) == true)
		count++;
	// South-West
	if (isApple(row + 1, col - 1, board) == true)
		count++;


	return (count);
}

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



