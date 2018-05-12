#pragma once
#include <string>
#include "SDL.h"
#include "Components.h"
#include "TextureManager.h"

class ColliderComponent : public Component
{
public:

	SDL_Rect collider;
	std::string tag; // Tag attach to a Entity to know what collides with the player

	SDL_Texture* tex;
	SDL_Rect srcR, destR;

	TransformComponent* transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int size)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::LoadTexture("assets/ColTex.png");
		srcR = { 0, 0, 32, 32 };
		destR = { collider.x, collider.y, collider.w, collider.h };

		
	}

	void draw() override
	{
		TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
	}

	void update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = (transform->width * transform->scale);
			collider.h = (transform->height * transform->scale);
		}
		if (tag == "player")
		{
			collider.x = static_cast<int>(transform->position.x + 20);
			collider.y = static_cast<int>(transform->position.y + 40);
			collider.w = (transform->width * transform->scale) - 40;
			collider.h = (transform->height * transform->scale) - 40;
		}
		destR.x = collider.x - Game::camera.x;
		destR.y = collider.y - Game::camera.y;
	}


};