#include "AssetManager.h"
#include "Components.h"

AssetManager::AssetManager(Manager* man) : manager(man)
{}

AssetManager::~AssetManager()
{}



void AssetManager::CreateBomb(Vector2D pos, std::string id)
{
	auto& bomb(manager->addEntity());
	bomb.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
	bomb.addComponent<SpriteComponent>(id, false);
	bomb.addComponent<BombComponent>();
	bomb.addComponent<ColliderComponent>("bomb");
	bomb.addGroup(Game::groupBomb);
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}


