#pragma once

#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include "AssetManager.h"

class SpriteComponent : public Component
{
private:
	TransformComponent * transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	bool animated = false; //  is it animated?
	int frames = 0; // number of frames
	int speed = 100; // Speed between frames in milliseconds


public:

	int animIndex = 0;

	std::map<const char*, Animation> animations; // hold our animation

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id) // Setting texture
	{
		setTex(id);
	}

	SpriteComponent(std::string id, bool isAnimatied) // Setting animation
	{
		animated = isAnimatied;

		Animation idle = Animation(0, 3, 200); // Index, Number of Frames, Speed
		Animation walk_Side = Animation(1, 6, 100);
		Animation walk_Front = Animation(2, 4, 100);
		Animation walk_Back = Animation(3, 4, 100);
		Animation idle_Side = Animation(4, 3, 200);
		Animation idle_Back = Animation(5, 3, 200);
		Animation eat_Cake = Animation(6, 8, 100);

		animations.emplace("Idle", idle);
		animations.emplace("Walk_Side", walk_Side);
		animations.emplace("Walk_Front", walk_Front);
		animations.emplace("Walk_Back", walk_Back);
		animations.emplace("Idle_Side", idle_Side);
		animations.emplace("Idle_Back", idle_Back);
		animations.emplace("Eat_Cake", eat_Cake);

		Play("Idle");
		setTex(id);
	}

	

	~SpriteComponent()
	{
		
	}

	void setTex(std::string id) // Allay us to Swap Textures 
	{
		texture = Game::assets->GetTexture(id);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
		
	}

	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;  
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override // Draw Method that Draw our Texture in Texture Manager
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}
	
	void Play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	 }
	void PlayAnim(const char* animName)
	{
		this->Play(animName);
	}
};