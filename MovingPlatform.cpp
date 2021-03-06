#include "MovingPlatform.h"
#include "MovingObject.h"
#include "Player.h"
#include <fstream>

MovingPlatform::MovingPlatform(float x, float y, int width, int height, char *textureSource, POS startPos, POS endPos, movingType moveType, float speed)
	:MovingObject(MOVING_PLATFORM, x, y, width, height, textureSource,  startPos, endPos, moveType, speed)
{
	// ctor
	setResizeable(true);
}

MovingPlatform::~MovingPlatform()
{
	// dtor
}

void MovingPlatform::update(float dt)
{
	MovingObject::update(dt);
}

void MovingPlatform::draw(void)
{
	// Beh�ver inte mer avancerad draw
	MovingObject::draw();
}

void MovingPlatform::saveToFile(std::ofstream *fout)
{

	*fout << getType() << " " << getID() << " " << (int)getX() << " " << (int)getY() << " " << (int)getStartPos().x << " " << (int)getStartPos().y << " " << (int)getEndPos().x << " " << (int)getEndPos().y << " ";
	*fout << getWidth() << " " << getHeight() << " " << getSpeed() << " ";
	*fout << getTextureSource() << endl;
}

void MovingPlatform::move(float dx, float dy)
{
	MovingObject::move(dx, dy);
}

void MovingPlatform::scale(direction side, int dwidth, int dheight)
{
	MovingObject::scale(side, dwidth, dheight);
}