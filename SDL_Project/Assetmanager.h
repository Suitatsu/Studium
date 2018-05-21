#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include "SDL_ttf.h"

class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();

	// gameobjects

	void createNumber(Vector2D pos, std::string id);
	void createBomb(Vector2D pos, std::string id);
	void createCake(Vector2D pos, std::string id);
	void createCakeWithTag(Vector2D pos, std::string id, std::string tag);

	//texture management
	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

	void AddFont(std::string id, std::string path, int fontSize);
	TTF_Font* GetFont(std::string id);

private:
	std::map<std::string, SDL_Texture*> textures;
	Manager* manager;
	std::map<std::string, TTF_Font*> fonts;
};