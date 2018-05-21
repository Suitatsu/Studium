#pragma once
#include "Game.h"
#include "Assetmanager.h"
#include "TextureManager.h"

class Splashscreen
{


public:


	int update()
	{
		SDL_Texture * splash_texture = TextureManager::LoadTexture("assets/splash.png");
		int splash_width, splash_height;

		SDL_QueryTexture(splash_texture, NULL, NULL, &splash_width, &splash_height);

		SDL_Rect splash_destination;

		splash_destination.x = 0;
		splash_destination.y = 0;
		splash_destination.w = splash_width;
		splash_destination.h = splash_height;

		SDL_RenderCopy(Game::renderer, splash_texture, NULL, &splash_destination);
		
		SDL_Delay(2000);
		return 1;
	}
	
	
};