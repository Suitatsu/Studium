#pragma once

#include "ECS.h"
#include "Components.h"


class BombComponent : public Component
{
private:
	TransformComponent * transform;


public:
	BombComponent() {}
	~BombComponent() {}

	void init() override 
	{
		transform = &entity->getComponent<TransformComponent>();
	}
	void update() override {}

};

