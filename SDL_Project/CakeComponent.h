#pragma once

#include "ECS.h"
#include "Components.h"


class CakeComponent : public Component
{
private:
	TransformComponent * transform;


public:
	CakeComponent() {}
	~CakeComponent() {}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}
	void update() override {}

};
