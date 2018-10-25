#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

#include "GameObject3D.h"

class Component;
class Entity;
struct GameData;
struct DrawData;

using ComponentID = std::size_t;

inline ComponentID getComponentID()
{
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getComponentID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Component() {};
	virtual ~Component() = default;
	
	Entity* m_entity;

	virtual void init() {};
	virtual void tick(GameData* gameData) {};
	virtual void draw(DrawData* drawData) {};

	
};

class Entity : public GameObject3D
{
public:
	Entity() {};
	~Entity() {};

	void tick(GameData* gameData);
	void draw(DrawData* drawData);

	bool isActive() { return m_isActive; }
	void destroy() { m_isActive = false; }

	template<typename T> bool hasComponent() const 
	{
		return m_componentBitSet[getComponentTypeID<T>];
	}

	template<typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs);

	template<typename T> T& getComponent() const
	{
		auto ptr = m_componentArray[getComponentTypeID<T>()];
		return *static_cast<T*>(ptr);
	}


private:
	bool m_isActive = true;
	std::vector<std::unique_ptr<Component>> m_compenents;

	ComponentArray m_componentArray;
	ComponentBitSet m_componentBitSet;
};

class Manager
{
public:
	void tick(GameData* gameData);
	void draw(DrawData* drawData);

	void refresh()
	{
		m_entities.erase(std::remove_if(std::begin(m_entities), std::end(m_entities), [](const std::unique_ptr<Entity> &mEntity)
		{
			return !mEntity->isActive();
	
		}), std::end(m_entities));
	}

	Entity& addEntity();
private:
	std::vector<std::unique_ptr<Entity>> m_entities;
};