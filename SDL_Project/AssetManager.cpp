#include "AssetManager.h"
#include "Components.h"

AssetManager::AssetManager(Manager* man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::createNumber(Vector2D pos, std::string id)
{
	auto& number(manager->addEntity());
	number.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
	number.addComponent<SpriteComponent>(id, false);
	number.addGroup(Game::groupNumber);
}

void AssetManager::createBomb(Vector2D pos, std::string id)
{
	auto& bomb(manager->addEntity());
	bomb.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
	bomb.addComponent<SpriteComponent>(id, false);
	bomb.addComponent<ColliderComponent>("bomb");
	bomb.addGroup(Game::groupBomb);

	
}

void AssetManager::createCake(Vector2D pos, std::string id)
{
	auto& cake(manager->addEntity());
	cake.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
	cake.addComponent<SpriteComponent>(id, false);
	cake.addComponent<ColliderComponent>("cake");
	cake.addGroup(Game::groupCake);
}

void AssetManager::createCakeWithTag(Vector2D pos, std::string id, std::string tag)
{
	auto& cake(manager->addEntity());
	cake.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
	cake.addComponent<SpriteComponent>(id, false);
	cake.addComponent<ColliderComponent>(tag);
	cake.addGroup(Game::groupCake);
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font * AssetManager::GetFont(std::string id)
{
	return fonts[id];
}


