#pragma once
#include "ECS.h"
#include "Assetmanager.h"
#include "Game.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UILabel : public Component
{
private:
	SDL_Rect position;
	std::string Text;
	std::string Font;
	SDL_Color Colour;
	SDL_Texture* Texture;

public:
	UILabel(int xpos, int ypos, std::string text, std::string font, SDL_Color& colour) 
	{
		Text = text;
		Font = font;
		Colour = colour;
		position.x = xpos;
		position.y = ypos;

		setText(Text, Font);
	}
	~UILabel() {}

	void setText(std::string text, std::string font)
	{	 
		SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->GetFont(font), text.c_str(), Colour); 
		Texture = SDL_CreateTextureFromSurface(Game::renderer, surf); 
		SDL_FreeSurface(surf);
		
		SDL_QueryTexture(Texture, nullptr, nullptr, &position.w, &position.h);
	} 
	 
	void draw() override   
	{ 
		SDL_RenderCopy(Game::renderer, Texture, nullptr, &position);
	}
};