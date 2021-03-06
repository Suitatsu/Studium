#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

class ColliderComponent;


class Game
{
private:
	int cnt;
	bool isRunning;
	SDL_Window *window;
	

public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean();

	static void AddTile(int id, int x, int y);
	static SDL_Renderer *renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
};

