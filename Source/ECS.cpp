#include "ECS.h"

template<typename T, typename ...TArgs>
inline T & Entity::addComponent(TArgs && ...mArgs)
{
	T* c(new T(std::forward<TArgs>(mArgs)...));
	c->entity = this;
	std::unique_ptr<Component> uPtr{c};
	m_compenents.emplace_back(std::move(uPtr));

	m_componentArray[getComponentTypeID<T>()] = c;
	m_componentArray[getComponentTypeID<T>()] = true;

	c->init();
	return *c;
}

void Entity::tick(GameData* gameData)
{
	for (auto& c : m_compenents)
	{
		c->tick(gameData);
	}

	GameObject3D::tick(gameData);
}

void Entity::draw(DrawData * drawData)
{
	for (auto& c : m_compenents)
	{
		c->draw(drawData);
	}
}

void Manager::tick(GameData * gameData)
{
	for (auto& e : m_entities)
	{
		e->tick(gameData);
	}
}

void Manager::draw(DrawData * drawData)
{
}

Entity& Manager::addEntity()
{
	Entity* e = new Entity();
	std::unique_ptr<Entity> uPtr{e};
	m_entities.emplace_back(std::move(uPtr));
	return *e;
}
