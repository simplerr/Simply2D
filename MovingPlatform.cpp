#include "MovingPlatform.h"
#include "MovingObject.h"
#include "Player.h"
#include <fstream>

MovingPlatform::MovingPlatform(float x, float y, int width, int height, char *textureSource, POS startPos, POS endPos, Player *player,  movingType moveType, float speed)
	:MovingObject(x, y, width, height, textureSource,  startPos, endPos, player, moveType, speed)
{
	// ctor
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
	// Behöver inte mer avancerad draw
	MovingObject::draw();
}

void MovingPlatform::saveToFile(std::ofstream *fout)
{

	*fout << getType() << " " << (int)getX() << " " << (int)getY() << " " << (int)getStartPos().x << " " << (int)getStartPos().y << " " << (int)getEndPos().x << " " << (int)getEndPos().y << " ";
	*fout << getWidth() << " " << getHeight() << " " << getSpeed() << " ";
	*fout << getTextureSource() << endl;
}

void MovingPlatform::move(float dx, float dy)
{
	MovingObject::move(dx, dy);
}

void MovingPlatform::scale(int dwidth, int dheight)
{
	MovingObject::scale(dwidth, dheight);
}