/*#include "DynamicObject.h"

DynamicObject::DynamicObject(float x, float y, int width, int height, char *textureSource, ObjectType type, Player *player)
	: Object(x, y, width, height, textureSource, type)
{
	mPlayer = player;
};

DynamicObject::~DynamicObject()
{
	delete mPlayer;
}

void DynamicObject::update(float dt)
{
	// to be inherited
}
bool DynamicObject::getPlayerCollision(void)
{
	RECT objectRect = getRect();
	RECT playerRect = mPlayer->getRect();

	if(!(playerRect.top >= objectRect.bottom ||  playerRect.bottom <= objectRect.top ||  playerRect.right <= objectRect.left ||  playerRect.left >= objectRect.right))
		return true;
	else
		return false;
}*/
