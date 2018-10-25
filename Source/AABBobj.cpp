#include "AABBobj.h"

void AABBobj::tick(GameData* gameData)
{
	CollidableObj::tick(gameData);
}

void AABBobj::draw(DrawData* drawData)
{
	if (m_drawBox)
	{
		//TODO: DrawBox
	}
}
