#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>


class AssetManager;
class ColliderComponent;


class Game
{
private:
	int cnt;
	

	

	
public:
	
	SDL_Window * window;

	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	
	static void faceDirection(int direction);
	

	void handleEvents();
	int update();
	bool running() { return isRunning; }
	void render();
	void clean();
	
	
	static SDL_Renderer *renderer;
	static SDL_Event event;
	static bool isRunning;
	static SDL_Rect camera;
	static AssetManager *assets;
	
	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupBomb,
		groupCake,
		groupTriggerbox,
		groupNumber
	};

};

