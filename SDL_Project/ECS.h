#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t; // ID for every Component like AI, Physics, Position, Inputhandling...
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()  // Creating the ComponentID and switch to the next
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept //Give the exact ID of the Component example: physics = ID1, Inputhandling = ID2...
{
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

// Tell if a Entity have a component attached
constexpr std::size_t maxComponents = 32;  // How much Components the Entity can max Hold
constexpr std::size_t maxGroups = 32;
using ComponentBitSet = std::bitset<maxComponents>; // Compare if the Entity has a selection of Components (Match)
using GroupBitset = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;  //array of Component pointers

class Component
{
public:

	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity
{
private:
	Manager & manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components; //List of all the Components the Entity is Holding

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitset groupBitset;

public:
	Entity(Manager& mManager) : manager(mManager) {}
	void update()
	{
		for (auto& c : components) c->update(); //for every Component use update through class:Manager
		
	}
	void draw() 
	{
		for (auto& c : components) c->draw();	//for every Component use draw through class:Manager
	}
	
	bool isActive() const { return active; }
	void destroy() { active = false; } //Remove inactive Entitys from the game through class:Manager

	bool hasGroup(Group mGroup)
	{
		return groupBitset[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		groupBitset[mGroup] = false;
	}

	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>  // add a component to our Entity
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();

		return *c;

	}

	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	
};

//List of all the Entitys
class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;  //create a List named entities
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
	void update()
	{
		for (auto& e : entities) e->update();  //update for every entity in our list
	}
	void draw()
	{
		for (auto& e : entities) e->draw();   //draw for every entity in our list
	}

	void refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v), [i](Entity* mEntity)
			{
				return !mEntity->isActive() || !mEntity->hasGroup(i);
			}),
				std::end(v));
		}

		
		entities.erase(std::remove_if(std::begin(entities), std::end(entities), //remove inactive Entitys from our game
			[](const std::unique_ptr<Entity> &mEntity)
		{
			return !mEntity->isActive();
		}),
			std::end(entities));
	}

	void AddToGroup(Entity* mEntity, Group mGroup)
	{

		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};
