#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include "GameState.h"

class KeyboardController : public Component
{
public:
	TransformComponent * transform;
	SpriteComponent * sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}
	void update() override
	{
		
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = -1; 
				Game::faceDirection(1);
				sprite->Play("Walk_Back");
				break;
			case SDLK_s:
				transform->velocity.y = 1; 
				Game::faceDirection(2);
				sprite->Play("Walk_Front"); 
				break;
			case SDLK_a:
				transform->velocity.x = -1; 
				Game::faceDirection(3);
				sprite->Play("Walk_Side"); 
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				
				break;
			case SDLK_d:
				transform->velocity.x = 1; 
				Game::faceDirection(4);
				sprite->spriteFlip = SDL_FLIP_NONE;
				sprite->Play("Walk_Side"); 
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = 0; 
				sprite->Play("Idle_Back"); 
				break;
			case SDLK_s:
				transform->velocity.y = 0; 
				sprite->Play("Idle"); 
				break;
			case SDLK_a:
				transform->velocity.x = 0; 
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				sprite->Play("Idle_Side"); 
				
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->spriteFlip = SDL_FLIP_NONE;
				sprite->Play("Idle_Side");
				break;
			
			default:
				break;
			}
		}


	}

};

